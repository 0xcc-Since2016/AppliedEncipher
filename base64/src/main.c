//AppliedEncipher--base64
//Learn with inj3ct0r
//in the world of Cryptology.

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>

const char* base64char = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

unsigned char map(unsigned char X){
	
	if(X == '+'){
		return (unsigned char)62;
	}
	if(X == '/'){
		return (unsigned char)63;
	}
	if('A' <= X && X <= 'Z'){
		return (unsigned char)(X-'A');
	}
	if('a' <= X && X<= 'z'){
		return (unsigned char)(X-'a'+26);
	}
	if('0' <= X && X <= '9'){
		return (unsigned char)(X-'0'+52);
	}else{
		
	}
}

char* convert(unsigned char c1 , unsigned char c2, unsigned char c3, unsigned char c4){
	
	char * res = (char*)malloc(sizeof(char)*4);
	unsigned char temp1 = map(c1);
	unsigned char temp2 = map(c2);
	unsigned char temp3 = map(c3);
	unsigned char temp4 = map(c4);
	res[0] = ((unsigned char)(temp1<<2)) | ((unsigned char)(temp2>>4));
	res[1] = ((unsigned char)(((unsigned char)(temp2)) & ((unsigned char)0x0F))<<4) | 
			 (unsigned char)(temp3>>2);
	res[2] = ((unsigned char)(((unsigned char)(temp3)) & ((unsigned char)0x03))<<6) | 
			 (unsigned char)temp4;
	res[3] = '\0';
	return res;
}

//Gen by alphabet of base64.


void base64_encode(const unsigned char* bindata, char* base64, int binlength){
	
	unsigned char curr;
	int i = 0;
	int j = 0;
	for(; i < binlength; i+=3 ){
		
		//encode Character one
		curr = (bindata[i]>>2);
		curr &= (unsigned char)0x3F;
		base64[j++] = base64char[(int)curr];
		//Put low 2 bit on 4-5 bit position.
		curr = ((unsigned char)(bindata[i] << 4)) & ((unsigned char) 0x30);
		
		if( i+1 >= binlength){
			
			base64[j++] = base64char[(int)curr];
			base64[j++] = '=';
			base64[j++] = '=';
			break;
		}
		//Merge 
		curr |= ((unsigned char)(bindata[i+1]>>4)) & ((unsigned char) 0x0F);
		base64[j++] = base64char[(int)curr];
		
		curr = ((unsigned char)(bindata[i+1]<<2)) & ((unsigned char) 0x3C);
		
		if( i+2 >= binlength){
			
			base64[j++] = base64char[(int)curr];
			base64[j++] = '=';
			break;
		}
		
		curr |= ((unsigned char)(bindata[i+2]>>6)) & ((unsigned char) 0x03);
		base64[j++] = base64char[(int)curr];
		
		//Get End 6 bit
		curr = ((unsigned char)bindata[i+2]) & ((unsigned char)0x3F);
		base64[j++] = base64char[(int)curr];
	}
	base64[j] = '\0';
}

void base64_decode(const unsigned char* encdata, char* base64, int enclength){

	int i = 0;
	int j = 0;
	
	//process the end.
	char* endSeg = (char*)malloc(sizeof(char)*4);
	if(encdata[enclength-1] == '=')
		{
			if(encdata[enclength-2] == '='){
				
				unsigned char t1 = map(encdata[enclength-3]);
				unsigned char t2 = map(encdata[enclength-4]);
				endSeg[0] = ((unsigned char)(t2<<2)) | (unsigned char)(t1>>4);
				endSeg[1] = '\0';
			}else{
				unsigned char t1 = map(encdata[enclength-2]);
				unsigned char t2 = map(encdata[enclength-3]);
				unsigned char t3 = map(encdata[enclength-4]);
				endSeg[0] = ((unsigned char)(t3<<2)) | (unsigned char)(t2>>4);
				endSeg[1] = ((unsigned char)((unsigned char)(t2 & 0x0F))<<4) | ((unsigned char)t1>>2);
				endSeg[2] = '\0';
			}
		}else{
				endSeg    = convert(encdata[enclength-4],encdata[enclength-3],
								   encdata[enclength-2],encdata[enclength-1]);
		}
	
	//process the body
	for(; i < enclength-4; i+=4){
		
		char * temp = convert(encdata[i],encdata[i+1],
							 encdata[i+2],encdata[i+3]);
		strncpy(base64+j,temp,3);
		j+=3;
	}
	
	strncpy(base64+j,endSeg,strlen(endSeg));
	j+= strlen(endSeg);
	base64[j] = '\0';
}


int main(){

	//test
	const unsigned char* P1 = "Bob sends Alice A message while Eve wants to break it.";
	const unsigned char* C1 = "Qm9iIHNlbmRzIEFsaWNlIEEgbWVzc2FnZSB3aGlsZSBFdmUgd2FudHMgdG8gYnJlYWsgaXQu";
	char* C = (char*)malloc((int)strlen(P1)*2);
	char* P = (char*)malloc((int)strlen(C1));
	base64_encode(P1,C,strlen(P1));
	base64_decode(C1,P,strlen(C1));
	printf("Length of PlainText: %d\n",strlen(P1));
	printf("Encipher:%s\n",C);
	printf("Length of CipherText: %d\n", strlen(C1));
	printf("Decipher:%s\n",P);
	return 0;
}
