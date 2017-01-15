#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <iterator>

using namespace std;

class termine{
	public:
		float coefficiente;
		bool incognita;
		int grado;
		
		termine(){
			grado=0;
			coefficiente=0;
			incognita=false;
		}
		termine(float f, bool b){
			coefficiente=f;
			incognita=b;
			if (incognita==true) grado=1;
			else grado=0;
		}
		termine(float f, bool b,int g){
			coefficiente=f;
			incognita=b;
			grado=g;
		}
};

class membro{
	public:
		vector<termine> contenuto;
		
		membro(){}
	  	membro(string s){
	  		int i=0;
	  		float coefficiente=1.0;
	  		bool incognita=false;
	  		string temp;
	  		temp.clear();
			while(s[i]!='\0'){
				if(s[i]=='1'||s[i]=='2'||s[i]=='3'||s[i]=='4'||s[i]=='5'||s[i]=='6'||s[i]=='7'||s[i]=='8'||s[i]=='9'||s[i]=='0'||s[i]=='+'||s[i]=='-'){
					do{
						temp+=s[i];
						i++;
					}
					while(s[i]!='\0'&&s[i]!='+'&&s[i]!='-'&&s[i]!='x'&&s[i]!='X');
					if((s[i]=='x'||s[i]=='X')&&(temp=="+"||temp=="-")) temp+='1';
					coefficiente = atof(temp.c_str());
				}
				if(s[i]=='x'||s[i]=='X'){
					incognita=true;
					i++;
				}
				contenuto.push_back(termine(coefficiente, incognita));
				incognita=false;
				coefficiente=1.0;
				temp.clear();
			}
			}
		void stampa(){
			int i;
			for(i=0;i<this->contenuto.size();i++){
    	    	if(this->contenuto[i].coefficiente>0) cout<<"+";             
     	    	cout<<this->contenuto[i].coefficiente;
     	    	if(this->contenuto[i].incognita==true) cout<<"x";
    	    }
			if(contenuto.size()==0) cout<<0;
		}
		void somma(){
			int i;
			float si=0,stn=0;
			for(i=0;i<this->contenuto.size();i++){
				if(contenuto[i].incognita==true) si+=contenuto[i].coefficiente;
				else stn+=contenuto[i].coefficiente;
			}
			contenuto.clear();
			if(si!=0||stn!=0){
				if(si!=0) contenuto.push_back(termine(si, true));
				if(stn!=0) contenuto.push_back(termine(stn, false));
			}
			else contenuto.push_back(termine(0, false));
		}
};

vector<string> SeparaMembriEquazione(string Equazione);
void RisolviEquazione(membro PrimoMembro, membro SecondoMembro);
void StampaPassaggio(membro PrimoMembro, membro SecondoMembro);
void Soluzione(membro PrimoMembro, membro SecondoMembro);
int MCD(int a, int b);

int main(){
	membro PrimoMembro, SecondoMembro;
	string EquazioneBase;
	vector<string> Membri(2);
	
	
	cout << "Inserire un equazione: ";
	cin >> EquazioneBase;
	//DA FARE: Controllo validita espressione inserita
	
	Membri=SeparaMembriEquazione(EquazioneBase);
	
	PrimoMembro=membro(Membri[0]);
	SecondoMembro=membro(Membri[1]);
	
	RisolviEquazione(PrimoMembro, SecondoMembro);
	cout<<endl;
	cout<<endl;
	system("PAUSE");
	return 0;
}

vector<string> SeparaMembriEquazione(string Equazione){
	int i,PosizioneUguale=0;
	char c;
	vector<string> temp(2);
	
	//Posiziona indicatore 'PosizioneUguale' sulla posizione successiva a '='
	while(c != '=') {
		c=Equazione[PosizioneUguale];
		PosizioneUguale++;
	}
	//temp[0] = primo membro dell'equazione
	for(i=0;i<=(PosizioneUguale-2);i++){
		temp[0]+=Equazione[i];
	}
	//temp[1] = secondo membro dell'equazione
	i=PosizioneUguale;
	for(;;){
		c=Equazione[i];
		if(c=='\0') break;
		temp[1]+=Equazione[i];
		i++;
	}
	return temp;
}

void RisolviEquazione(membro PrimoMembro, membro SecondoMembro){
	int i=0;
	int nTermPM = PrimoMembro.contenuto.size();
	vector<termine>::iterator FineVettorePM = PrimoMembro.contenuto.end();
	
	while(i<nTermPM){
		if(PrimoMembro.contenuto[(nTermPM-1-i)].incognita==false){
			SecondoMembro.contenuto.push_back(termine(PrimoMembro.contenuto[nTermPM-1-i].coefficiente*-1, false));
			PrimoMembro.contenuto.erase(FineVettorePM-i-1);
		}
		i++;
	}
	
	i=0;
	int nTermSM = SecondoMembro.contenuto.size();
	vector<termine>::iterator FineVettoreSM = SecondoMembro.contenuto.end();
	
	while(i<nTermSM){
		if(SecondoMembro.contenuto[(nTermSM-1-i)].incognita==true){
			PrimoMembro.contenuto.push_back(termine(SecondoMembro.contenuto[nTermSM-1-i].coefficiente*-1, true));
			SecondoMembro.contenuto.erase(FineVettoreSM-1-i);
		}
		i++;
	}
	
	StampaPassaggio(PrimoMembro, SecondoMembro);
	
	PrimoMembro.somma();
	SecondoMembro.somma();
	
	StampaPassaggio(PrimoMembro, SecondoMembro);
	
	Soluzione(PrimoMembro, SecondoMembro);
}

void StampaPassaggio(membro PrimoMembro, membro SecondoMembro){
	PrimoMembro.stampa();
	cout<<"=";
	SecondoMembro.stampa();
	cout<<endl;
}

void Soluzione(membro PrimoMembro,membro SecondoMembro){
	float N=SecondoMembro.contenuto[0].coefficiente;
	float D=PrimoMembro.contenuto[0].coefficiente;
	int MD;
	
	if(D==0){
		if (N==0){
			cout<<"Equazione Indeterminata"<<endl;
			exit(0);
		}
		cout<<"Equazione Impossibile"<<endl;
		exit(0);
	}
	
	
	if((N-(long)N)==0&&(D-(long)D)==0){             //risultato intero
		
		MD=MCD(N,D);
		cout<<"x="<<N<<"/"<<D;
		if (MD!=1) cout<<"="<<N/MD<<"/"<<D/MD;
		cout<<"="<<N/D;
	}
	else{                                           //risultato decimale
		cout<<"x="<<N/D<<endl;
	}
}

int MCD(int a, int b) {
	if (b==0) return a;
	else return MCD(b, a%b);
}






