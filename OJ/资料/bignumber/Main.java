import java.util.*;
import java.math.*;
public class Main{
    public static void main(String args[]){
       Scanner cin = new Scanner(System.in);
       BigInteger a, b;
      
       //���ļ�EOF����
       while (cin.hasNext()){
           a = cin.nextBigInteger();
           b = cin.nextBigInteger();
          
           System.out.println(a.add(b)); //�������ӷ�
           System.out.println(a.subtract(b)); //����������
           System.out.println(a.multiply(b)); //�������˷�
           System.out.println(a.divide(b)); //����������(ȡ��)
           System.out.println(a.remainder(b)); //������ȡģ
          
           //�������ıȽ�
           if( a.compareTo(b) == 0 ) System.out.println("a == b"); //������a==b
           else if( a.compareTo(b) > 0 ) System.out.println("a > b"); //������a>b
           else if( a.compareTo(b) < 0 ) System.out.println("a < b"); //������a<b
          
           //����������ֵ
           System.out.println(a.abs()); //������a�ľ���ֵ
          
           //����������
           int exponent=10;
           System.out.println(a.pow(exponent)); //������a��exponent����
          
           //���ش�����ʮ���Ƶ��ַ�����ʾ
           System.out.println(a.toString());
          
           //���ش�����p���Ƶ��ַ�����ʾ
           int p=8;
           System.out.println(a.toString(p));
       }
    }
}