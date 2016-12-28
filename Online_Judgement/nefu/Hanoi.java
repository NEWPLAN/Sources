import java.io.*;
import java.util.*;

public class Hanoi{

    public final static int  MAX_NUM = 105;
    public final static long MAX_VAL = 1L << 60;

	public static int  p[] = new int[MAX_NUM];
	public static long f[] = new long[MAX_NUM];
	public static long step;

    public static long dfs(int n){
		if(f[n] != -1L) return f[n];
	    if(n == 1){		
			f[n] = 1;
			p[n] = 1;
		    return 1;
		}
		long minval = MAX_VAL;
		int  curval = 0;
		for(int i = 1; i < n; i++){
		    long tmp = 2 * dfs(i) + (1L << (n - i)) - 1;
			if(tmp < minval){
				curval = n - i; 
				minval = tmp;
			}
		}
		p[n] = curval;
		return f[n] = minval;
	}

	public static void Move3(int n, char a, char b, char c){
	    step++;
		if(n == 1){
		    System.out.println(a+"--->"+c);
			return;
		}
		n--;
		Move3(n, a, c, b);
		System.out.println(a+"--->"+c);
		Move3(n, b, a, c);
	}

	public static void Move4(int n, char a, char b, char c, char d){
	    if(n == 1){
		    System.out.println(a+"--->"+d);
			step++;
			return;
		}
		Move4(n - p[n], a, c, d, b);
		Move3(p[n], a, c, d);
		Move4(n - p[n], b, a, c, d);
	}

	public static void main(String[] args){
		Scanner cin = new Scanner(System.in);
		while(cin.hasNextInt()){
			 int n = cin.nextInt();
			 Arrays.fill(p, -1);
			 Arrays.fill(f, -1);
			 step = 0;
			 dfs(n);
			 Move4(n, 'A', 'B', 'C', 'D');
		     System.out.println(step);
        }		
	}
}
