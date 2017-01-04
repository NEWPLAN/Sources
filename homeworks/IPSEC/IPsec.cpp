#include "sysinclude.h"

extern struct ipsecSpdHeader *g_ipsec_spd_in;
extern struct ipsecSpdHeader *g_ipsec_spd_out;


extern void ipsec_Receive_process(char *pBuffer, int dwLen);
extern void ipsec_Send_process(char *pBuffer, int iLen);
extern AH_ALGORITHM *ahAlgorithmLookup(unsigned char algAuth);
extern ESP_ALGORITHM *espAlgorithmLookup(unsigned char  algEncry);
extern int ah4CalculateICV(unsigned char *pPacketCpy, unsigned short  totalLen, unsigned char  *pOutput,	const AH_ALGORITHM  *pAlg, IPSEC_ASSOC  *ipsecSA);
extern int espAuthCalculate(unsigned char  *pData, unsigned short  dataLen, unsigned char  *pOutput, AH_ALGORITHM  *pAlg, IPSEC_ASSOC  *ipsecSA);


/*define global variables*/
const int IPSEC_HEAD_LEN = sizeof(IPSEC_HEAD);//the length of ip head is 20
const int IPSEC_AH_LEN = 24;//the length of AH is 24
const int IPSEC_AH_HEAD_LEN = sizeof(IPSEC_AH);	//the length of AH head is 12
bool ESPFlags = false;//global for ESP flags


int CheckIP(unsigned int src, unsigned int dst, unsigned int mask)
{
	return (src & mask) == (dst & mask);
}
//	calculate the checksum
unsigned short GetCheckSum(IPSEC_HEAD * iphead)
{
	unsigned int checksum = 0;
	checksum += (unsigned int)((iphead->verlen) << 8);
	checksum += (unsigned int)(iphead->tos);
	checksum += (unsigned int)(ntohs(iphead->tot_len));
	checksum += (unsigned int)(ntohs(iphead->id));
	checksum += (unsigned int)(ntohs(iphead->frag_off));
	checksum += (unsigned int)((iphead->ttl) << 8);
	checksum += (unsigned int)(iphead->protocol);
	checksum += (unsigned int)(ntohl(iphead->saddr) >> 16);
	checksum += (unsigned int)(ntohl(iphead->saddr) & 0xFFFF);
	checksum += (unsigned int)(ntohl(iphead->daddr) >> 16);
	checksum += (unsigned int)(ntohl(iphead->daddr) & 0xFFFF);
	while (checksum >> 16)checksum = (checksum & 0xFFFF) + (checksum >> 16);
	checksum = ~checksum;
	return (unsigned short) checksum;
}


//	decode AH stream
int decodeAHPackage(char * &pBuffer, unsigned int &length, IPSEC_POLICY_SELECTOR *pSP)
{
	if (length < IPSEC_HEAD_LEN + IPSEC_AH_LEN)return -1;
	int lengthData = length - IPSEC_AH_LEN;
	char *newDataBuffer = (char*)malloc(sizeof(char) * lengthData);
	memset(newDataBuffer, 0, lengthData);

	memcpy(newDataBuffer, pBuffer, IPSEC_HEAD_LEN);
	IPSEC_HEAD * iphead = (IPSEC_HEAD *) newDataBuffer;

	IPSEC_AH *ipsecAH = (IPSEC_AH *) (pBuffer + IPSEC_HEAD_LEN);	//	get AH head
	IPSEC_ASSOC *ipsecSA = pSP->sa;//	get SA
	while (ipsecSA)
	{
		if (!ipsecSA)return -1;/*can not been NULL*/
		if (CheckIP(ipsecSA->srcAddr, ntohl(iphead->saddr), 0xFFFF)
		        && CheckIP(ipsecSA->dstAddr, ntohl(iphead->daddr), 0xFFFF)
		        && ipsecSA->proto == IPSEC_PROTO_AH && ipsecSA->spi == ntohl(ipsecAH->ah_spi))
			break;
		ipsecSA = ipsecSA->next;
	}

	AH_ALGORITHM * pALG = ahAlgorithmLookup((unsigned char)ipsecSA->algAuth);//	get AH algorithm
	if (pALG == NULL)return -1;
	//	authorite AH
	char * pPacketCpy = (char*)malloc(sizeof(char) * length);
	memcpy(pPacketCpy, pBuffer, length);
	char * pOutput = (char*)malloc(sizeof(char) * (IPSEC_AH_LEN - IPSEC_AH_HEAD_LEN));
	memset(pOutput, 0, IPSEC_AH_LEN - IPSEC_AH_HEAD_LEN);
	//	calculate the digist info
	if (ah4CalculateICV((unsigned char *)pPacketCpy, length, (unsigned char *)pOutput, pALG, ipsecSA) != 0)return -1;
	if (memcmp(pBuffer + IPSEC_HEAD_LEN + IPSEC_AH_HEAD_LEN, pOutput, IPSEC_AH_LEN - IPSEC_AH_HEAD_LEN) != 0)return -1;
	//	decode AH pack
	iphead->protocol = ipsecAH->ah_nxt;
	iphead->tot_len = htons(lengthData);
	iphead->check = htons(GetCheckSum(iphead));

	memcpy(newDataBuffer + IPSEC_HEAD_LEN, pBuffer + IPSEC_HEAD_LEN + IPSEC_AH_LEN, lengthData - (IPSEC_AH_LEN - IPSEC_HEAD_LEN));/*padding data*/
	pBuffer = newDataBuffer;
	length = lengthData;
	return 0;
}

//	decode ESP pack
int decodeESPPackage(char * &pBuffer, unsigned int &length, IPSEC_POLICY_SELECTOR *pSP)
{
	ESPFlags = true;
	IPSEC_HEAD * iphead = (IPSEC_HEAD *) pBuffer;
	IPSEC_ESP * pESP = (IPSEC_ESP *) (pBuffer + IPSEC_HEAD_LEN);
	IPSEC_ASSOC *ipsecSA = pSP->sa;//	get SA

	while (ipsecSA)
	{
		if (!ipsecSA)return -1;/*can not been NULL*/
		if (CheckIP(ipsecSA->srcAddr, ntohl(iphead->saddr), 0xFFFF)
		        && CheckIP(ipsecSA->dstAddr, ntohl(iphead->daddr), 0xFFFF)
		        && ipsecSA->proto == IPSEC_PROTO_ESP && ipsecSA->spi == ntohl(pESP->spi))
			break;
		ipsecSA = ipsecSA->next;
	}
	AH_ALGORITHM *pALG = ahAlgorithmLookup((unsigned char)ipsecSA->algAuth);//	get ESP authentory algorithm
	if (pALG == NULL)return -1;

	ESP_ALGORITHM * pESP_ALG = espAlgorithmLookup((unsigned char)ipsecSA->algEncry);	//	get ESP encrypt algorithm
	if (pESP_ALG == NULL)return -1;

	//	recover ESP data
	char * pOutput = (char*)malloc(sizeof(char) * IPSEC_AH_HEAD_LEN);
	memset(pOutput, 0, IPSEC_AH_HEAD_LEN);
	if (espAuthCalculate((unsigned char*) (pBuffer + IPSEC_HEAD_LEN), length - IPSEC_HEAD_LEN - IPSEC_AH_HEAD_LEN, (unsigned char*)pOutput, pALG, ipsecSA) != 0)return -1;
	if (memcmp(pBuffer + length - IPSEC_AH_HEAD_LEN, pOutput, IPSEC_AH_HEAD_LEN) != 0)return -1;
	unsigned int encryLen = 0, plainLen = 0;
	char * plainText = NULL, *encryText = NULL;
	char * key1 = (char *) ipsecSA->keyEncry->key;
	if (ipsecSA->algEncry == IPSEC_ENCRY_DES)
	{
		encryLen = length - IPSEC_HEAD_LEN - 8 - 12 - 8;
		plainText = (char*)malloc(sizeof(char) * encryLen);
		encryText = (char*)malloc(sizeof(char) * encryLen);
		memcpy(encryText, pBuffer + 20 + 8 + 8, encryLen);
		char iv[8];
		memcpy(iv, pBuffer + 20 + 8, 8);	// playload, padding
		if (pESP_ALG->decrypt((unsigned char*)encryText, encryLen, (unsigned char*) plainText, &plainLen, key1, iv) != 0)return -1;
	}
	else if (ipsecSA->algEncry == IPSEC_ENCRY_3DES)
	{
		encryLen = length - IPSEC_HEAD_LEN - 8 - 12;
		plainText = (char*)malloc(sizeof(char) * encryLen);
		encryText = (char*)malloc(sizeof(char) * encryLen);
		memcpy(encryText, pBuffer + 20 + 8, encryLen);
		char * key2 = (char *) ipsecSA->keyEncry2->key;
		if (pESP_ALG->decrypt((unsigned char*)encryText, encryLen, (unsigned char*) plainText, &plainLen, key1, key2) != 0)return -1;
	}
	//	decode ESP pack
	char *newDataBuffer = NULL;
	ESP_TAIL * pESP_TAIL = (ESP_TAIL*) (plainText + plainLen - 2);
	unsigned int dataLen = plainLen - 2 - pESP_TAIL->padLen;
	if (ipsecSA->mode == IPSEC_MODE_TUNNEL)
	{
		newDataBuffer = (char*)malloc(sizeof(char) * dataLen);
		memset(newDataBuffer, 0, sizeof(char) * dataLen);
		memcpy(newDataBuffer, plainText, dataLen);
		length = dataLen;
	}
	else
	{
		newDataBuffer = (char*)malloc(sizeof(char) * (dataLen + sizeof(IPSEC_HEAD)));
		memset(newDataBuffer, 0, sizeof(char) * (dataLen + sizeof(IPSEC_HEAD)));
		memcpy(newDataBuffer, pBuffer, IPSEC_HEAD_LEN);
		memcpy(newDataBuffer + IPSEC_HEAD_LEN, plainText, dataLen);
		iphead = (IPSEC_HEAD*) newDataBuffer;
		iphead->protocol = pESP_TAIL->nxtHeader;
		iphead->tot_len = htons((unsigned short)(IPSEC_HEAD_LEN + dataLen));
		iphead->check = GetCheckSum(iphead);
		length = dataLen + IPSEC_HEAD_LEN;
	}
	pBuffer = newDataBuffer;
	return 0;
}

//	encode  AH package packAH
int encodeAHPackage(char * &pBuffer, unsigned int &length, IPSEC_ASSOC *ipsecSA)
{
	int lengthData = length + IPSEC_AH_LEN;
	char * newDataBuffer = (char*)malloc(sizeof(char) * lengthData);
	memset(newDataBuffer, 0, lengthData);
	memcpy(newDataBuffer, pBuffer, IPSEC_HEAD_LEN);/*add ip head*/
	IPSEC_HEAD *iphead = (IPSEC_HEAD *) newDataBuffer;
	IPSEC_AH *ipsecAH = (IPSEC_AH *)malloc(sizeof(IPSEC_AH));
	ipsecAH->ah_nxt = iphead->protocol;
	ipsecAH->ah_len = 0x04;	//	IPSEC_AH_LEN / 4 - 2 = 4
	ipsecAH->ah_reserved = 0;
	ipsecAH->ah_spi = htonl(ipsecSA->spi);
	ipsecAH->ah_seq = 0;

	iphead->protocol = IPT_AUTH;
	iphead->tot_len = htons((unsigned short)lengthData);
	iphead->check = htons(GetCheckSum(iphead));
	memcpy(newDataBuffer + IPSEC_HEAD_LEN, (unsigned char*)ipsecAH, IPSEC_AH_HEAD_LEN);
	memcpy(newDataBuffer + IPSEC_HEAD_LEN + IPSEC_AH_LEN, pBuffer + IPSEC_HEAD_LEN, length - IPSEC_HEAD_LEN);

	char *pPacketCpy = (char*)malloc(sizeof(char) * lengthData);
	memset(pPacketCpy, 0, lengthData);
	memcpy(pPacketCpy, newDataBuffer, lengthData);
	char *pOutput = (char*)malloc(sizeof(char) * (IPSEC_AH_LEN - IPSEC_AH_HEAD_LEN));
	memset(pOutput, 0, IPSEC_AH_LEN - IPSEC_AH_HEAD_LEN);

	//	get AH algorithm and authority
	AH_ALGORITHM *pAlg = ahAlgorithmLookup((unsigned char)ipsecSA->algAuth);
	if (ah4CalculateICV((unsigned char*)pPacketCpy, lengthData, (unsigned char*)pOutput, pAlg, ipsecSA) != 0)return -1;
	memcpy(newDataBuffer + IPSEC_HEAD_LEN + IPSEC_AH_HEAD_LEN, pOutput, IPSEC_AH_HEAD_LEN);
	pBuffer = newDataBuffer;
	length = lengthData;
	return 0;
}

//	encode ESP packages
int encodeESPPackage(char * &pBuffer, unsigned int &length, IPSEC_ASSOC *ipsecSA)
{
	IPSEC_HEAD *iphead = (IPSEC_HEAD *)pBuffer;
	IPSEC_ESP *pESP = (IPSEC_ESP *)malloc(sizeof( IPSEC_ESP));
	pESP->spi = htonl(ipsecSA->spi);
	pESP->seq = 0;
	ESP_ALGORITHM *pESP_ALG = espAlgorithmLookup((unsigned char)ipsecSA->algEncry);//	get ESP encryption algorithms
	unsigned char * pPaddingText, * pEncryText, * pOriginalText, nxtHeader, pOutput[12];
	unsigned int encryLen, plainLen, originalLen;
	char * newDataBuffer = NULL, iv[8], *data = NULL ;
	int lengthData;
	AH_ALGORITHM *pAlg = NULL;
	char * key1 = (char *) ipsecSA->keyEncry->key;
	char * key2 = (char *) ipsecSA->keyEncry2->key;

	if (ipsecSA->mode == IPSEC_MODE_TUNNEL)
	{
		pOriginalText = (unsigned char *)pBuffer;
		originalLen = length;
		nxtHeader = 0x04;
	}
	else if (ipsecSA->mode == IPSEC_MODE_TRANSPORT)
	{
		pOriginalText = (unsigned char *)pBuffer + IPSEC_HEAD_LEN;
		originalLen = length - IPSEC_HEAD_LEN;
		nxtHeader = iphead->protocol;
	}

	memcpy(iv, (char *)(ipsecSA->iv), 8);
	pESP_ALG->padding((unsigned char*)pOriginalText, originalLen, &pPaddingText, &plainLen, nxtHeader);
	pEncryText = (unsigned char*)malloc(sizeof(unsigned char) * plainLen);

	switch (ipsecSA->algEncry)/*encrypt pack with ESP*/
	{
	case IPSEC_ENCRY_DES:
		pESP_ALG->encrypt(pPaddingText, plainLen, pEncryText, &encryLen, key1, iv);
		data = (char*)malloc(sizeof(char) * (encryLen + 8 + 8));
		memcpy(data, (char *) pESP, 8);
		memcpy(data + 8, (char *)(ipsecSA->iv), 8);
		memcpy(data + 16, (char *)pEncryText, encryLen);
		pAlg = ahAlgorithmLookup((unsigned char)ipsecSA->algAuth);
		espAuthCalculate((unsigned char *)data, encryLen + 16, pOutput, pAlg, ipsecSA);
		lengthData = IPSEC_HEAD_LEN + 8 + 8 + encryLen + 12;
		newDataBuffer = (char*)malloc(sizeof(char) * lengthData);
		memcpy(newDataBuffer, pBuffer, IPSEC_HEAD_LEN);
		memcpy(newDataBuffer + IPSEC_HEAD_LEN, (char *)data, encryLen + 8 + 8);
		memcpy(newDataBuffer + lengthData - 12, (char*)pOutput, 12);
		break;
	case IPSEC_ENCRY_3DES:
		pESP_ALG->encrypt((unsigned char *)pPaddingText, plainLen, (unsigned char *)pEncryText, &encryLen, key1, key2);
		data = (char*)malloc(sizeof(char) * (encryLen + 8));
		memcpy(data, (char*) pESP, 8);
		memcpy(data + 8, (char *)pEncryText, encryLen);

		pAlg = ahAlgorithmLookup((unsigned char)ipsecSA->algAuth);
		espAuthCalculate((unsigned char *)data, encryLen + 8, pOutput, pAlg, ipsecSA);
		lengthData = IPSEC_HEAD_LEN + 8 + encryLen + 12;
		newDataBuffer = (char*)malloc(sizeof(char) * lengthData);
		memcpy(newDataBuffer, pBuffer, IPSEC_HEAD_LEN);
		memcpy(newDataBuffer + IPSEC_HEAD_LEN, (char *)data, encryLen + 8);
		memcpy(newDataBuffer + lengthData - 12, (char*)pOutput, 12);
		break;
	default:
		break;

	}

	iphead = (IPSEC_HEAD *)newDataBuffer;/*padding the ip package*/
	iphead->saddr = htonl(ipsecSA->srcAddr);
	iphead->daddr = htonl(ipsecSA->dstAddr);
	iphead->tot_len = htons((unsigned short)lengthData);
	iphead->protocol = IPT_ESP;
	iphead->check = htons(GetCheckSum(iphead));
	pBuffer = newDataBuffer;
	length = lengthData;
	return 0;
}

/*decode IPsec package*/
int stud_ipsec_input(char * pBuffer, unsigned int length)
{
	IPSEC_HEAD * iphead = (IPSEC_HEAD *) pBuffer;
	unsigned int daddr = ntohl(iphead->daddr), saddr = ntohl(iphead->saddr); /*convert the stream from net to localhost*/
	unsigned short checksum = ntohs(iphead->check);
	IPSEC_POLICY_SELECTOR *pSelector = NULL;
	if (NULL == g_ipsec_spd_in)return 1;

	pSelector = g_ipsec_spd_in->spSelector;
	while (pSelector)
	{
		if (CheckIP(pSelector->srcAddr, saddr, pSelector->srcPref) && CheckIP(pSelector->dstAddr, daddr, pSelector->dstPref))
			break;/*match the exactly ip*/
		pSelector = pSelector->next;
	}
	if (!pSelector || !pSelector->sa) return 1; /*failed to get the spd and sad*/

	int success = 0;
	while (true)
	{
		if (pSelector->sa == NULL || success == -1)	break;
		if (iphead->protocol == IPT_AUTH)
		{
			success = decodeAHPackage(pBuffer, length, pSelector);
			if (success != 0)return 1;
		}
		else if (iphead->protocol == IPT_ESP)
		{
			success = decodeESPPackage(pBuffer, length, pSelector);
			if (success != 0)return 1;
		}
		else break;
		iphead = (IPSEC_HEAD*) pBuffer;/*get the next buffer*/
	}
	if (success != 0)return 1;/*any problem in deal with the package*/
	ipsec_Receive_process(pBuffer, length);
	return 0;
}

//	 encode IPsec package
int stud_ipsec_output(char * pBuffer, unsigned int length)
{
	IPSEC_HEAD * iphead = (IPSEC_HEAD *) pBuffer;
	unsigned int saddr = ntohl(iphead->saddr), daddr = ntohl(iphead->daddr); /*convert the stream from net to localhost*/
	IPSEC_POLICY_SELECTOR *pSelector = NULL;
	if (g_ipsec_spd_out == NULL)return 1;

	pSelector = g_ipsec_spd_out->spSelector;/*get spd*/
	while (pSelector)
	{
		if (CheckIP(pSelector->srcAddr, saddr, pSelector->srcPref)
		        && CheckIP(pSelector->dstAddr, daddr, pSelector->dstPref))
		{
			if (ESPFlags && pSelector->sa->proto != IPSEC_PROTO_ESP)
			{
				pSelector = pSelector->next;
				continue;
			}
			break;/*match the exactly ip*/
		}
		pSelector = pSelector->next;
	}
	if (!pSelector) return 1;/*failed to get the spd*/

	IPSEC_ASSOC * ipsecSA = pSelector->sa;
	if (ipsecSA == NULL)return 1;
	switch (ipsecSA->proto)
	{
	case IPSEC_PROTO_AH:
		if (encodeAHPackage(pBuffer, length, ipsecSA) != 0)return 1; break;
	case IPSEC_PROTO_ESP:
		if (encodeESPPackage(pBuffer, length, ipsecSA) != 0)return 1;
		ipsecSA = pSelector->sa;
		while (ipsecSA)/*mix module*/
		{
			if (ipsecSA->proto != IPSEC_PROTO_AH)
			{
				ipsecSA = ipsecSA->next;
				continue;
			}
			if (encodeAHPackage(pBuffer, length, ipsecSA) != 0)return 1;
			break;
		}
		break;
	default:
		return 1; break;
	}
	ipsec_Send_process(pBuffer, length);
	return 0;
}
