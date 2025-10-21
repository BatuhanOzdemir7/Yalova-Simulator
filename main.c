#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <time.h>
#include <conio.h>

#define SATIR 19
#define SUTUN 81

typedef struct {
    char harf;         // Haritada g�r�nen harf (�rn. '�')
    char isim[20];     // Kent ad�
    int butce;
    float vergi;         // % cinsinden (�rn. 5 = %5)
    int aylikGelir;
    int asker;
    float memnun;
    int bizimMi;       // 1 = oyuncunun y�netti�i kent, 0 = de�il
} Kent;

#define KENT_SAYISI 10

Kent kentler[KENT_SAYISI] = {
    {'�', "��narc�k", 500, 5, 50, 10, 100, 0},
    {'Y', "Yalova",   500, 5, 50, 10, 100, 0},
    {'S', "Samanl�",  500, 5, 50, 10, 100, 0},
    {'E', "Elmal�k",500, 5, 50, 10, 100, 0},
    {'T', "Termal", 500, 5, 50, 10, 100, 0},
    {'A', "Alt�nova",   500, 5, 50, 10, 100, 0},
    {'K', "Kocaeli",  500, 5, 50, 10, 100, 0},
    {'F', "F�st�kl�",  500, 5, 50, 10, 100, 0},
    {'B', "Bursa",    500, 5, 50, 10, 100, 0},
    {'\0', "",        0,   0,  0,  0, 100, 0} // sonland�r�c� (gerekirse)
};

void guncelleKentler() {
    for (int i = 0; i < KENT_SAYISI; i++) {
        Kent* k = &kentler[i];

        if (k->bizimMi) {
            int gelir = (k->aylikGelir * k->vergi) / 100;
            k->butce += gelir;
        } else {
            k->vergi = rand() % 26; // 0-25 aras� yeni vergi oran�

            if (rand() % 2 == 0 && k->butce >= 250) {
                k->asker+=4;
                k->butce -= 100;
            }

            int gelir = (k->aylikGelir * k->vergi) / 100;
            k->butce += gelir;
        }

        // Memnuniyet vergiyle azal�r
        k->memnun -= 0.1 * k->vergi;
        if (k->memnun < 0) k->memnun = 0;
        if (k->memnun > 100) k->memnun = 100;

        // Asker kayb�: memnuniyetsizlik oran�na g�re
        int memnuniyetsizlik = 100 - k->memnun;
        int sans = rand() % 100;
        if (sans < memnuniyetsizlik && k->asker > 0) {
            k->asker--;
        }
    }
}



void harita(char matris[SATIR][SUTUN]) {
    system("cls"); // Windows i�in ekran temizle
    for (int i = 0; i < SATIR; i++) {
        for (int j = 0; j < SUTUN - 1; j++) { // -1 ��nk� \0 var
            putchar(matris[i][j]);
        }
        putchar('\n');
    }
}

void kentSec(char kent, char kontrolEdilenKent){
    system("cls");

    printf("====== KENT B�LG�S� ======\n");

    int bulundu = 0;
    for (int i = 0; i < KENT_SAYISI; i++) {
        if (kentler[i].harf == kent) {
            bulundu = 1;
            printf("�ehir: %s\n", kentler[i].isim);
            printf("B�t�e: %d\n", kentler[i].butce);
            printf("Vergi Oran�: %.3f%%\n", kentler[i].vergi);
            printf("Ayl�k Gelir: %d\n", kentler[i].aylikGelir);
            printf("Asker Say�s�: %d\n", kentler[i].asker);
            printf("Memnuniyet Oran�: %.3f%%\n", kentler[i].memnun);

            if (kentler[i].bizimMi) {
                printf("\n>> Bu kent sizin kontrol�n�zde.\n");

                // Geli�tirilebilir: oyuncuya se�enekler sunabilirsiniz
                printf("\nY�netim Se�enekleri:\n");
                printf("1. Vergi oran�n� de�i�tir\n");
                printf("2. Asker �ret (25 birim maliyet)\n");
                printf("3. Sava� a�\n");
                printf("Se�iminiz (yoksa bir tu�a bas�n): ");

                char secim = getch();
                if (secim == '1') {
                    printf("\nYeni vergi oran�n� girin (0-100): ");
                    int yeniVergi;
                    scanf("%d", &yeniVergi);
                    if (yeniVergi >= 0 && yeniVergi <= 100) {
                        kentler[i].vergi = yeniVergi;
                        printf("Vergi oran� g�ncellendi.\n");
                    } else {
                        printf("Ge�ersiz oran!\n");
                    }
                }
                else if (secim == '2') {
                    if (kentler[i].butce >= 25) {
                        kentler[i].asker += 1;
                        kentler[i].butce -= 25;
                        printf("Asker �retildi.\n");
                    } else {
                        printf("Yetersiz b�t�e.\n");
                    }
                }
                else if (secim == '3') {
                    printf("\nHangi kente sava� a�mak istiyorsunuz? Harfini giriniz: ");
                    char hedefHarf;
                    scanf(" %c", &hedefHarf);

                    int hedefIndex = -1;
                    int bizimIndex = -1;

                    for (int i = 0; i < KENT_SAYISI; i++) {
                        if (kentler[i].harf == hedefHarf) hedefIndex = i;
                        if (kentler[i].harf == kent) bizimIndex = i;
                    }

                    if (hedefIndex == -1 || hedefIndex == bizimIndex) {
                        printf("Ge�ersiz �ehir.\n");
                    } else if (kentler[hedefIndex].bizimMi) {
                        printf("Bu kent zaten sizin kontrol�n�zde.\n");
                    } else {
                        int a = kentler[bizimIndex].asker;
                        int d = kentler[hedefIndex].asker;

                        if (a == 0) {
                            printf("Hi� askeriniz yok! Sava� a�amazs�n�z.\n");
                        } else {
                            float sans = (float)a / (a + d) * 100;
                            int r = rand() % 100;

                            printf("\nSava� ba�lad�! Kazanma �ans�n�z: %.1f%%\n", sans);
                            if (r < sans) {
                                printf("Tebrikler! %s kentini ele ge�irdiniz.\n", kentler[hedefIndex].isim);
                                kentler[hedefIndex].bizimMi = 1;
                                kentler[hedefIndex].asker = a / 2; // kalan askerle ba�lat
                                kentler[bizimIndex].asker = a / 2;
                            } else {
                                printf("Sava�� kaybettiniz. Askerlerinizin tamam� yok oldu.\n");
                                kentler[bizimIndex].asker = 0;
                            }
                        }
                    }

                    printf("\nDevam etmek i�in bir tu�a bas�n...");
                    getch();
                }


            } else {
                printf("\n>> Bu kent sizin kontrol�n�zde de�il.\n");
            }
            break;
        }
    }

    if (!bulundu) {
        printf("Bilinmeyen �ehir: %c\n", kent);
    }

    printf("\nDevam etmek i�in bir tu�a bas�n...");
    getch();
}



int main() {
    setlocale(LC_ALL, "Turkish");
    srand(time(NULL));  // rand i�in
    time_t sonGuncelleme = time(NULL);
    char kontrolEdilenKent = '\0';  // hen�z y�netilen kent yok

    char map[SATIR][SUTUN] = {
        "                                           ========   ====         ==========       ",
        "                                                     =                ===  =         ",
        "                                                  ======         ============        ",
        "                                           ==========A======K================        ",
        "                                       ======================================        ",
        "                   ======�========Y==========================================        ",
        "                 ============================================================        ",
        "               ================S====E========================================        ",
        "            ===============T=================================================        ",
        "        =====================================================================        ",
        "      =======================================================================        ",
        "      =======================================================================        ",
        "            F==================================  ============================        ",
        "             ==========   ===========B======                  ===============        ",
        "                   =        ===============                    ==============        ",
        "                             ===============   ==                 ===========        ",
        "      ===                ============================         ===============        ",
        "==============           ====================================================        ",
        "=============================================================================        "
    };

    printf("Karakterinizin ad�n� giriniz (tek karakter): ");
    char karakter = getchar();

    // Ba�lang�� pozisyonu, �rne�in 5.sat�r 29.s�tun (haritadaki '=' karakterlerinden biri)
    int x = 5, y = 29;
    map[x][y] = karakter;

    while(1){
        // ZAMAN KONTROL� BURADA
        time_t simdi = time(NULL);
        if (difftime(simdi, sonGuncelleme) >= 5.0) {
            guncelleKentler();
            sonGuncelleme = simdi;
        }

        // Kent harflerini sabitle
        map[3][53] = 'A';
        map[3][60] = 'K';
        map[5][25] = '�';
        map[5][34] = 'Y';
        map[7][31] = 'S';
        map[7][36] = 'E';
        map[8][27] = 'T';
        map[12][12] = 'F';
        map[13][37] = 'B';

        harita(map);
        printf("Hareket etmek i�in WASD tu�lar�n� kullan�n�z (��kmak i�in q): ");

        char yon = getch();

        if (yon == 'q' || yon == 'Q') break;

        // Yeni pozisyon i�in ge�ici de�i�kenler
        int nx = x;
        int ny = y;

        switch(yon){
            case 'w': case 'W':
                nx = x - 1; // yukar�
                break;
            case 's': case 'S':
                nx = x + 1; // a�a��
                break;
            case 'a': case 'A':
                ny = y - 1; // sola
                break;
            case 'd': case 'D':
                ny = y + 1; // sa�a
                break;
            default:
                continue; // ge�ersiz tu�sa d�ng�ye devam et
        }

        // Harita s�n�rlar� i�inde mi?
        if(nx < 0 || nx >= SATIR || ny < 0 || ny >= SUTUN -1) continue;

        // Yeni pozisyonda karakter hareket edilebilir mi kontrol�
        // Mesela sadece '=' karakteri �zerinden hareket edilsin
        if(map[nx][ny] == '=') {
            // Eski pozisyonu '=' yap
            map[x][y] = '=';

            // Yeni pozisyona karakteri koy
            map[nx][ny] = karakter;

            // Koordinatlar� g�ncelle
            x = nx;
            y = ny;
        }
        else if(map[nx][ny] != '=' && map[nx][ny] != ' '){
                char hedef = map[nx][ny];
            // Eski pozisyonu '=' yap
            map[x][y] = '=';

            // Koordinatlar� g�ncelle
            x = nx;
            y = ny;
            if (kontrolEdilenKent == '\0') {
                kontrolEdilenKent = hedef;
                for (int i = 0; i < KENT_SAYISI; i++) {
                    if (kentler[i].harf == hedef) {
                        kentler[i].bizimMi = 1;
                        break;
                    }
                }
            }
        time_t simdi = time(NULL);
        if (difftime(simdi, sonGuncelleme) >= 5.0) {
            guncelleKentler();
            sonGuncelleme = simdi;
        }

            kentSec(hedef, kontrolEdilenKent);
        }
    }

    printf("Oyun sonland�r�ld�.\n");
    return 0;
}
