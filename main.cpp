#include<SFML/Window.hpp>
#include<SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include<iostream>
#include<fstream>
#include"Button.h"
#include<string>
#include<sstream>
#include<vector>



struct celula{
    float x,y;///coord
    int nrvecini,vecini[7];

    int booleana;///tip celula 1 - basic   -1 - selectat rosu  2 - selectat galben 3 - selectat albastru
    int areobstacol;///0 gol 1 inamic melee 2 erou 3 blocat 4 portal 5 vrajitoare


    bool CheckL(sf::RenderWindow &window){///verifica daca dai click stanga pe celula
        if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
            sf::Vector2i pos=sf::Mouse::getPosition(window);
                if(pos.x>=x-52&&pos.y>=y-62&&pos.x<=x+52&&pos.y<=y+62)
                    return true;
        }
        return false;
    }
    bool CheckR(sf::RenderWindow &window){///verifica daca dai click dreapta pe celula
        if(sf::Mouse::isButtonPressed(sf::Mouse::Right)){
            sf::Vector2i pos=sf::Mouse::getPosition(window);
                if(pos.x>=x-52&&pos.y>=y-62&&pos.x<=x+52&&pos.y<=y+62)
                    return true;
        }
        return false;
    }
};

celula v[62];



bool Selectat=false;
int nrselectat=0;
unsigned int runde=1;

bool InTimpulAnimatiei=false;

int xerou,yerou,pozerou;
int viataerou=7;
bool poatemerge=true;
bool poateataca=true;

bool mergejocu=true;

std::vector<int> vecmelee;
std::vector<int> vecvraji;
std::vector<int> vecvrajiinitial;
std::vector<int> vecvrajisterse;
std::vector<int> vecmeleeinitial;
std::vector<int> vecmeleesterse;



int alglee[62],coada[1000];

int GameState=0;///0 main menu 1 joc pr-zis
int NivelCurent=1;


void Schimba(int i) {///selecteaza sau deselecteaza
    if(v[i].booleana==1&&Selectat==false){///pt a selecta argumentul trb sa fie hexul respectiv
        for(int j=1;j<=v[i].nrvecini;j++){
            if(poatemerge&&(v[v[i].vecini[j]].areobstacol==0||v[v[i].vecini[j]].areobstacol==4))
                v[v[i].vecini[j]].booleana=2;
            if(poateataca&&(v[v[i].vecini[j]].areobstacol==1||v[v[i].vecini[j]].areobstacol==5))
                v[v[i].vecini[j]].booleana=3;
        }
        Selectat=true;
        nrselectat=i;
    }
    else{///dar pentru deselectare e bun orice argument
            v[nrselectat].booleana=1;
            for(int j=1;j<=v[nrselectat].nrvecini;j++)
                v[v[nrselectat].vecini[j]].booleana=1;
            Selectat=false;
            nrselectat=0;

    }
}

int Directie(int x1,int y1,int x2,int y2){///iau doua pozitii si spun in care cadran e prima fata de a doua 1 nord-vest,2 nord-est,3 sud-est,4 sud-vest
    if(x2<=x1&&y2<=y1)
        return 1;
    if(x2<=x1&&y2>=y1)
        return 2;
    if(x2>=x1&&y2>=y1)
        return 3;
    if(x2>=x1&&y2<=y1)
        return 4;
}

///matrice nivele

short int nivele[100][70];

void NextLevel(){
    vecmelee.clear();
    vecvraji.clear();
    for(int i=1;i<=61;i++){
        v[i].areobstacol=nivele[NivelCurent][i];
        if(v[i].areobstacol==2){
            xerou=v[i].x;
            yerou=v[i].y;
            pozerou=i;
        }
        if(v[i].areobstacol==1)
            vecmelee.push_back(i);
        if(v[i].areobstacol==5)
            vecvraji.push_back(i);
    }
    NivelCurent++;
}







int main() {

    std::vector<sf::VideoMode> availiableVideoModes;
    availiableVideoModes = sf::VideoMode::getFullscreenModes();


    sf::RenderWindow window(availiableVideoModes[0],"Nu cred ca vezi asta",sf::Style::Fullscreen);

    sf::View view(sf::Vector2f(960.0f,540.0f),sf::Vector2f(1920.0f,1080.0f));







    std::ifstream cin;
    cin.open("input/coordonate.in");
    for(int i=1;i<=61;i++){
        cin>>v[i].x>>v[i].y>>v[i].nrvecini;
        for(int j=1;j<=v[i].nrvecini;j++)
            cin>>v[i].vecini[j];
        v[i].booleana=1;
    }
    cin.close();

    std::ifstream fin("input/nivel1.in");
    int nrniv;
    fin>>nrniv;
    for(int i=1;i<=nrniv;i++)
        for(int j=1;j<=61;j++)
            fin>>nivele[i][j];
    fin.close();




    sf::SoundBuffer b1,b2,b3,b4,b5;
    b1.loadFromFile("sunete/atac.ogg");
    b2.loadFromFile("sunete/pasi.wav");
    b3.loadFromFile("sunete/buton.wav");
    b4.loadFromFile("sunete/vrajitoare.ogg");
    b5.loadFromFile("sunete/damageai.wav");

    sf::Sound a1,a2,a3,a4,a5;
    a1.setBuffer(b1);
    a2.setBuffer(b2);
    a3.setBuffer(b3);
    a4.setBuffer(b4);
    a5.setBuffer(b5);

    sf::Music m1;
    m1.openFromFile("sunete/gameover.ogg");
    m1.setLoop(true);


    sf::Texture t1,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17,t18,t19,t20,t21,t22;
    t1.loadFromFile("texturi/hextilev3.png");
    t3.loadFromFile("texturi/selectat2.png");
    t4.loadFromFile("texturi/knight.png");
    t5.loadFromFile("texturi/erou2.png");
    t6.loadFromFile("texturi/buton.png");
    t7.loadFromFile("texturi/selectat3.png");
    t8.loadFromFile("texturi/gameover.png");
    t9.loadFromFile("texturi/inima.png");
    t10.loadFromFile("texturi/blocat.png");
    t11.loadFromFile("texturi/portal.png");
    t12.loadFromFile("texturi/vrajitoare.png");
    t13.loadFromFile("texturi/butonStart.png");
    t14.loadFromFile("texturi/save.png");
    t15.loadFromFile("texturi/continue.png");
    t16.loadFromFile("texturi/mainmenu.png");
    t17.loadFromFile("texturi/popup.png");
    t18.loadFromFile("texturi/cancel.png");
    t19.loadFromFile("texturi/sure.png");
    t20.loadFromFile("texturi/background.png");
    t21.loadFromFile("texturi/butonEdit.png");
    t22.loadFromFile("texturi/export.png");

    sf::Sprite s1,s3,s4,s5,s6,s7,s8,s9,s10,s11,s12,s13;

    s1.setTexture(t1);
    s3.setTexture(t3);
    s4.setTexture(t4);
    s5.setTexture(t5);
    s6.setTexture(t7);
    s7.setTexture(t8);
    s8.setTexture(t9);
    s9.setTexture(t10);
    s10.setTexture(t11);
    s11.setTexture(t12);
    s12.setTexture(t17);
    s13.setTexture(t20);

    s1.setOrigin(66.0f,57.5f);
    s3.setOrigin(66.0f,57.5f);
    s4.setOrigin(66.0f,57.5f);
    s5.setOrigin(66.0f,57.5f);
    s6.setOrigin(66.0f,57.5f);
    s9.setOrigin(66.0f,57.5f);
    s10.setOrigin(66.0f,57.5f);
    s11.setOrigin(66.0f,57.5f);
    s12.setOrigin(300.0f,200.0f);
    s13.setOrigin(960.0f,540.0f);

    s7.setOrigin(240.0f,180.0f);
    s8.setOrigin(24.0f,24.0f);


    s8.setPosition(sf::Vector2f(1870.0f,40.0f));
    s7.setPosition(sf::Vector2f(960.0f,540.0f));
    s12.setPosition(sf::Vector2f(960.0f,540.0f));
    s13.setPosition(sf::Vector2f(960.0f,540.0f));

    Button buton(&t6,sf::Vector2f(1600.0f,800.0f),200,100);

    Button butonStart(&t13,sf::Vector2f(810.0f,430.0f),300,150);

    Button butonSave(&t14,sf::Vector2f(1600.0f,650.0f),200,100);

    Button butonContinue(&t15,sf::Vector2f(810.0f,630.0f),300,150);

    Button butonMenu(&t16,sf::Vector2f(1600.0f,500.0f),200,100);

    Button butonCancel(&t18,sf::Vector2f(1060.0f,600.0f),100,75);

    Button butonSure(&t19,sf::Vector2f(760.0f,600.0f),100,75);

    Button butonEdit(&t21,sf::Vector2f(810.0f,830.0f),300,150);

    Button butonExport(&t22,sf::Vector2f(1600.0f,800.0f),200,100);


    sf::Font font;
    font.loadFromFile("fonturi/font.ttf");

    sf::Text text,text2;
    text.setFont(font);
    text.setString("Round:");
    text.setCharacterSize(40);
    text.setFillColor(sf::Color::Black);

    text2.setFont(font);
    text2.setCharacterSize(40);
    text2.setFillColor(sf::Color::Black);
    text2.setPosition(sf::Vector2f(128.0f,0.0f));

    inceput:

    ///reinitializez toate variabilele globale din cauza goto ului

    Selectat=false;
    nrselectat=0;
    runde=1;
    viataerou=7;
    poatemerge=true;
    poateataca=true;
    mergejocu=true;
    GameState=0;
    NivelCurent=1;
    InTimpulAnimatiei=false;
    NextLevel();///primul nivel


    sf::Clock clock;
    double timer=0,timer2=0,timer3=0,delay=0.09;


    ///
    ///game loop
    ///



    while(window.isOpen()){
        sf::Event evnt;
        while(window.pollEvent(evnt))
            switch(evnt.type){
            case sf::Event::Closed:
                window.close();
                break;
            }

    sf::Color as(100,100,100);
    window.clear(as);

    if(GameState==0){///meniu principal


        if(butonStart.Check(window)){
            a3.play();
            GameState=1;
        }

        if(butonContinue.Check(window)){
            a3.play();
            int temp;
            std::ifstream in("input/savefile.txt");
            if(in>>temp){
                in>>temp;
                while(NivelCurent<temp){
                    NextLevel();
                    NivelCurent++;
                }
                for(int i=1;i<=61;i++)
                    in>>v[i].areobstacol;
                in>>poateataca;
                in>>poatemerge;
                in.close();
                GameState=1;
            }
        }

        if(butonEdit.Check(window)){
            a3.play();
            GameState=3;
            for(int i=1;i<=61;i++)
                v[i].areobstacol=0;
        }



        window.draw(s13);
        butonContinue.Draw(window);
        butonStart.Draw(window);
        butonEdit.Draw(window);
    }

    if(GameState==3){/// level editor

        double time=clock.getElapsedTime().asSeconds();
        clock.restart();
        timer3+=time;

        if(timer3>delay){

            timer3=0;

            for(int i=1;i<=61;i++){
                if(v[i].CheckL(window)){
                    v[i].areobstacol++;
                    if(v[i].areobstacol>5)
                        v[i].areobstacol=0;
                }

            }

            if(butonExport.Check(window)){
                a3.play();
                std::ofstream out("input/export.txt");
                for(int i=1;i<=61;i++)
                    out<<v[i].areobstacol<<" ";
            }
        }
        butonExport.Draw(window);
        for(int i=1;i<=61;i++){

            s1.setPosition(sf::Vector2f(v[i].x,v[i].y));
            window.draw(s1);

            if(v[i].areobstacol==1){
                s4.setPosition(sf::Vector2f(v[i].x,v[i].y));
                window.draw(s4);
            }
            if(v[i].areobstacol==2){
                s5.setPosition(sf::Vector2f(v[i].x,v[i].y));
                window.draw(s5);
            }
            if(v[i].areobstacol==3){
                s9.setPosition(sf::Vector2f(v[i].x,v[i].y));
                window.draw(s9);
            }
            if(v[i].areobstacol==4){
                s10.setPosition(sf::Vector2f(v[i].x,v[i].y));
                window.draw(s10);
            }
            if(v[i].areobstacol==5){
                s11.setPosition(sf::Vector2f(v[i].x,v[i].y));
                window.draw(s11);
            }

        }

    }



    if(GameState==2){///cand apesi pe butonul main menu in joc

        if(butonCancel.Check(window)){
            a3.play();
            GameState=1;
            sf::sleep(sf::milliseconds(300));
        }

        if(butonSure.Check(window)){
            a3.play();
            GameState=0;
            sf::sleep(sf::milliseconds(300));

            ///inapoi la conditiile initiale
          goto inceput;
        }



        for(int i=1;i<=61;i++){

            s1.setPosition(sf::Vector2f(v[i].x,v[i].y));
            window.draw(s1);

            if(v[i].booleana==2){
                s3.setPosition(sf::Vector2f(v[i].x,v[i].y));
                window.draw(s3);
            }
            if(v[i].booleana==3){
                s6.setPosition(sf::Vector2f(v[i].x,v[i].y));
                window.draw(s6);
            }
            if(v[i].areobstacol==1){
                s4.setPosition(sf::Vector2f(v[i].x,v[i].y));
                window.draw(s4);
            }
            if(v[i].areobstacol==2){
                s5.setPosition(sf::Vector2f(v[i].x,v[i].y));
                window.draw(s5);
            }
            if(v[i].areobstacol==3){
                s9.setPosition(sf::Vector2f(v[i].x,v[i].y));
                window.draw(s9);
            }
            if(v[i].areobstacol==4){
                s10.setPosition(sf::Vector2f(v[i].x,v[i].y));
                window.draw(s10);
            }
            if(v[i].areobstacol==5){
                s11.setPosition(sf::Vector2f(v[i].x,v[i].y));
                window.draw(s11);
            }
        }
        buton.Draw(window);
        butonSave.Draw(window);
        butonMenu.Draw(window);

        window.draw(text);
        window.draw(text2);

        for(int i=1;i<=viataerou;i++){
            window.draw(s8);
            sf::Vector2f poz=s8.getPosition();
            poz.x-=50;
            s8.setPosition(poz);
        }
        s8.setPosition(sf::Vector2f(1870.0f,40.0f));


        if(mergejocu==false)
            window.draw(s7);


        window.draw(s12);///pop up
        butonSure.Draw(window);
        butonCancel.Draw(window);


    }




    if(GameState==1){///in timpu jocului

         double time=clock.getElapsedTime().asSeconds();
            clock.restart();
            timer+=time;
            if(InTimpulAnimatiei)
                timer2+=time*2;
            if(timer2>1){
                InTimpulAnimatiei=false;
                timer2=0;
            }
            if(delay<timer){
                timer=0;
                for(int i=1;i<=61;i++){

                    if(v[i].CheckL(window)&&Selectat&&v[i].booleana!=2&&v[i].booleana!=3){///deselectezi daca apesi oriunde mai putin unde poti merge
                        Schimba(i);
                    }
                    if(v[i].CheckL(window)&&v[i].areobstacol==2&&Selectat==false){///selectezi daca apesi eroul
                        Schimba(i);
                    }
                    if(v[i].CheckL(window)&&v[i].booleana==2&&v[i].areobstacol==0&&poatemerge==true){///cand selectezi, te poti misca pe un hex galben, daca n-are vrajitoare pe el
                        v[nrselectat].areobstacol=0;
                        Schimba(i);///din cauza la cum e fct functia, poti pune orice argument
                        v[i].areobstacol=2;
                        poatemerge=false;
                        xerou=v[i].x;
                        yerou=v[i].y;
                        pozerou=i;
                        a2.play();
                    }
                    if(v[i].CheckL(window)&&v[i].booleana==3&&poateataca==true){///ataca
                        poateataca=false;
                        Schimba(i);
                        a1.play();
                        if(v[i].areobstacol==1)
                            vecmelee.erase(std::find(vecmelee.begin(),vecmelee.end(),i));
                        if(v[i].areobstacol==5)
                            vecvraji.erase(std::find(vecvraji.begin(),vecvraji.end(),i));
                        v[i].areobstacol=0;
                        a5.play();
                    }

                    if(v[i].CheckL(window)&&v[i].booleana==2&&v[i].areobstacol==4&&poatemerge==true){
                        NextLevel();
                        for(int i=1;i<=61;i++)
                            v[i].booleana=1;
                        if(viataerou<10)
                            viataerou++;
                    }


                }

                ///
                ///buton save
                ///

                if(butonSave.Check(window)){
                    a3.play();
                    std::ofstream out("input/savefile.txt");
                    out<<1<<" ";
                    out<<NivelCurent<<" ";
                    for(int i=1;i<=61;i++)
                        out<<v[i].areobstacol<<" ";
                    out<<poateataca<<" "<<poatemerge;
                    out.close();
                }


                ///
                ///!!!buton save
                ///


                ///buton main menu


                if(butonMenu.Check(window)){
                    m1.stop();
                    a3.play();
                    GameState=2;
                }


                ///
                ///butonu next
                ///


                if(buton.Check(window)||(poateataca==false&&poatemerge==false)||(vecmelee.empty()&&vecvraji.empty()&&poatemerge==false)){///apesi butonu, se termina runda sau, daca te ai si miscat si ai si atacat
                    a3.play();
                    InTimpulAnimatiei=true;
                    Schimba(0);///deselectezi erou cand apesi butonul, din cauza la cum e fct functia, poti pune orice argument
                    vecvrajiinitial=vecvraji;
                    vecmeleeinitial=vecmelee;
                    vecvrajisterse.clear();
                    vecmeleesterse.clear();

                    for(auto i=0;i<vecvraji.size();i++){///lee pentru vrajitoare
                        int pozinamic=vecvraji[i];
                        int pp=1;
                        for(int j=1;j<=v[pozinamic].nrvecini;j++){///daca vrajitoare e langa erou, ad damage
                            if(v[v[pozinamic].vecini[j]].areobstacol==2){
                                viataerou--;
                                pp=0;
                                a4.play();
                                for(int q=1;q<=v[pozerou].nrvecini;q++){
                                    if(v[v[pozerou].vecini[q]].areobstacol==1){
                                        v[v[pozerou].vecini[q]].areobstacol=0;
                                        vecmelee.erase(std::find(vecmelee.begin(),vecmelee.end(),v[pozerou].vecini[q]));
                                        vecmeleesterse.push_back(v[pozerou].vecini[q]);
                                    }
                                    if(v[v[pozerou].vecini[q]].areobstacol==5){
                                        v[v[pozerou].vecini[q]].areobstacol=0;
                                        vecvraji.erase(std::find(vecvraji.begin(),vecvraji.end(),v[pozerou].vecini[q]));
                                        vecvrajisterse.push_back(v[pozerou].vecini[q]);
                                    }
                                }
                                a5.play();
                            }
                        }

                        if(viataerou==0){
                            mergejocu=false;
                            m1.play();
                        }


                        if(pp==1){
                            for(int j=1;j<=61;j++)
                                if(v[j].areobstacol>2&&v[j].areobstacol!=5)
                                    alglee[j]=-1;
                                else
                                    alglee[j]=0;

                            alglee[pozinamic]=1;

                            coada[1]=pozinamic;
                            int inceput=1,sfarsit=1;
                            while(inceput<=sfarsit){
                                for(int j=1;j<=v[coada[inceput]].nrvecini;j++){
                                    if(alglee[v[coada[inceput]].vecini[j]]==0){
                                        sfarsit++;
                                        coada[sfarsit]=v[coada[inceput]].vecini[j];
                                        alglee[v[coada[inceput]].vecini[j]]=alglee[coada[inceput]]+1;
                                    }
                                }
                                inceput++;

                            }
                            if(alglee[pozerou]!=0){
                                int lcurenta=alglee[pozerou];
                                int poz=pozerou;
                                if(lcurenta==3){///atac vrajitoare de la distanta
                                    viataerou--;
                                    a4.play();
                                    for(int q=1;q<=v[pozerou].nrvecini;q++){
                                        if(v[v[pozerou].vecini[q]].areobstacol==1){
                                            v[v[pozerou].vecini[q]].areobstacol=0;
                                            vecmelee.erase(std::find(vecmelee.begin(),vecmelee.end(),v[pozerou].vecini[q]));
                                        }
                                        if(v[v[pozerou].vecini[q]].areobstacol==5){
                                            v[v[pozerou].vecini[q]].areobstacol=0;
                                            vecvraji.erase(std::find(vecvraji.begin(),vecvraji.end(),v[pozerou].vecini[q]));
                                        }
                                    }
                                    a5.play();
                                }
                                else{
                                    while(lcurenta>2){
                                        for(int q=1;q<=v[poz].nrvecini;q++)
                                            if(alglee[v[poz].vecini[q]]==lcurenta-1){
                                                lcurenta--;
                                                poz=v[poz].vecini[q];
                                                break;
                                        }
                                    }
                                    if(v[poz].areobstacol==0){
                                        vecvraji[i]=poz;
                                        v[pozinamic].areobstacol=0;
                                        v[poz].areobstacol=5;
                                    }
                                }
                            }
                        }
                    }
                    for(auto i=0;i<vecmelee.size();i++){///lee pentru inamici melee
                        int pozinamic=vecmelee[i];
                        int pp=1;
                        for(int j=1;j<=v[pozinamic].nrvecini;j++){
                            if(v[v[pozinamic].vecini[j]].areobstacol==2){
                                viataerou--;
                                pp=0;
                                a1.play();
                            }
                        }

                        if(viataerou==0){
                            mergejocu=false;
                            m1.play();
                        }


                        if(pp==1){
                            for(int j=1;j<=61;j++)
                                if(v[j].areobstacol>2&&v[j].areobstacol!=5)
                                    alglee[j]=-1;
                                else
                                    alglee[j]=0;

                            alglee[pozinamic]=1;

                            coada[1]=pozinamic;
                            int inceput=1,sfarsit=1;
                            while(inceput<=sfarsit){
                                for(int j=1;j<=v[coada[inceput]].nrvecini;j++){
                                    if(alglee[v[coada[inceput]].vecini[j]]==0){
                                        sfarsit++;
                                        coada[sfarsit]=v[coada[inceput]].vecini[j];
                                        alglee[v[coada[inceput]].vecini[j]]=alglee[coada[inceput]]+1;
                                    }
                                }
                                inceput++;

                            }
                            if(alglee[pozerou]!=0){
                                int lcurenta=alglee[pozerou];
                                int poz=pozerou;
                                while(lcurenta>2){
                                    for(int q=1;q<=v[poz].nrvecini;q++)
                                        if(alglee[v[poz].vecini[q]]==lcurenta-1){
                                            lcurenta--;
                                            poz=v[poz].vecini[q];
                                            break;
                                    }
                                }
                                if(v[poz].areobstacol==0){
                                    vecmelee[i]=poz;
                                    v[pozinamic].areobstacol=0;
                                    v[poz].areobstacol=1;
                                }
                            }



                        }

                    }

                    poatemerge=true;
                    poateataca=true;

                    for(auto i=0;i<vecvrajisterse.size();i++)///creez vectori cu vrajitoare care s au miscat pt animatie
                        vecvrajiinitial.erase(std::find(vecvrajiinitial.begin(),vecvrajiinitial.end(),vecvrajisterse[i]));

                    for(auto i=0;i<vecmeleesterse.size();i++)
                        vecmeleeinitial.erase(std::find(vecmeleeinitial.begin(),vecmeleeinitial.end(),vecmeleesterse[i]));

                }
            }

            ///
            /// !!!butonu next
            ///


            ///
            ///desenare
            ///



        for(int i=1;i<=61;i++){

            s1.setPosition(sf::Vector2f(v[i].x,v[i].y));
            window.draw(s1);

            if(v[i].booleana==2){
                s3.setPosition(sf::Vector2f(v[i].x,v[i].y));
                window.draw(s3);
            }
            if(v[i].booleana==3){
                s6.setPosition(sf::Vector2f(v[i].x,v[i].y));
                window.draw(s6);
            }
            if(v[i].areobstacol==2){
                s5.setPosition(sf::Vector2f(v[i].x,v[i].y));
                window.draw(s5);
            }
            if(v[i].areobstacol==3){
                s9.setPosition(sf::Vector2f(v[i].x,v[i].y));
                window.draw(s9);
            }
            if(v[i].areobstacol==4){
                s10.setPosition(sf::Vector2f(v[i].x,v[i].y));
                window.draw(s10);
            }
        }

        if(InTimpulAnimatiei){
            for(auto i=0;i<vecvrajiinitial.size();i++){
                int x1=v[vecvrajiinitial[i]].x;
                int y1=v[vecvrajiinitial[i]].y;
                int x2=v[vecvraji[i]].x;
                int y2=v[vecvraji[i]].y;

                int dir=Directie(x1,y1,x2,y2);

                int xmin=std::min(x1,x2);
                int ymin=std::min(y1,y2);
                int xmax=std::max(x1,x2);
                int ymax=std::max(y1,y2);
                int difx=abs(x1-x2);
                int dify=abs(y1-y2);
                int xtemp,ytemp;

                switch(dir){
                    case 1:
                        xtemp=xmax-difx*timer2;
                        ytemp=ymax-dify*timer2;
                        break;
                    case 2:
                        xtemp=xmax-difx*timer2;
                        ytemp=ymin+dify*timer2;
                        break;
                    case 3:
                        xtemp=xmin+difx*timer2;
                        ytemp=ymin+dify*timer2;
                        break;
                    case 4:
                        xtemp=xmin+difx*timer2;
                        ytemp=ymax-dify*timer2;
                        break;

                }
                s11.setPosition(sf::Vector2f(xtemp,ytemp));
                window.draw(s11);

            }

            for(auto i=0;i<vecmeleeinitial.size();i++){
                int x1=v[vecmeleeinitial[i]].x;
                int y1=v[vecmeleeinitial[i]].y;
                int x2=v[vecmelee[i]].x;
                int y2=v[vecmelee[i]].y;

                int dir=Directie(x1,y1,x2,y2);

                int xmin=std::min(x1,x2);
                int ymin=std::min(y1,y2);
                int xmax=std::max(x1,x2);
                int ymax=std::max(y1,y2);
                int difx=abs(x1-x2);
                int dify=abs(y1-y2);
                int xtemp,ytemp;


                switch(dir){
                    case 1:
                        xtemp=xmax-difx*timer2;
                        ytemp=ymax-dify*timer2;
                        break;
                    case 2:
                        xtemp=xmax-difx*timer2;
                        ytemp=ymin+dify*timer2;
                        break;
                    case 3:
                        xtemp=xmin+difx*timer2;
                        ytemp=ymin+dify*timer2;
                        break;
                    case 4:
                        xtemp=xmin+difx*timer2;
                        ytemp=ymax-dify*timer2;
                        break;

                }


                s4.setPosition(sf::Vector2f(xtemp,ytemp));
                window.draw(s4);

            }
        }
        else{
            for(int i=1;i<=61;i++){
                if(v[i].areobstacol==1){
                    s4.setPosition(sf::Vector2f(v[i].x,v[i].y));
                    window.draw(s4);
                }
                if(v[i].areobstacol==5){
                    s11.setPosition(sf::Vector2f(v[i].x,v[i].y));
                    window.draw(s11);
                }

            }
        }

        buton.Draw(window);
        butonSave.Draw(window);
        butonMenu.Draw(window);

        window.draw(text);
        window.draw(text2);

        for(int i=1;i<=viataerou;i++){
            window.draw(s8);
            sf::Vector2f poz=s8.getPosition();
            poz.x-=50;
            s8.setPosition(poz);
        }
        s8.setPosition(sf::Vector2f(1870.0f,40.0f));


        if(mergejocu==false)
            window.draw(s7);
    }
    window.setView(view);
    window.display();

    ///
    /// !!!desenare
    ///

    }
    return 0;
}
