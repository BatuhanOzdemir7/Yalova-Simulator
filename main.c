#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <time.h>
#include <conio.h>

#define SATIR 19
#define SUTUN 81

typedef struct {
    char harf;         // Haritada görünen harf (örn. 'Ç')
    char isim[20];     // Kent adý
    int butce;
    float vergi;         // % cinsinden (örn. 5 = %5)
    int aylikGelir;
    int asker;
    float memnun;
    int bizimMi;       // 1 = oyuncunun yönettiði kent, 0 = deðil
} Kent;

#define KENT_SAYISI 10

Kent kentler[KENT_SAYISI] = {
    {'Ç', "Çýnarcýk", 500, 5, 50, 10, 100, 0},
    {'Y', "Yalova",   500, 5, 50, 10, 100, 0},
    {'S', "Samanlý",  500, 5, 50, 10, 100, 0},
    {'E', "Elmalýk",500, 5, 50, 10, 100, 0},
    {'T', "Termal", 500, 5, 50, 10, 100, 0},
    {'A', "Altýnova",   500, 5, 50, 10, 100, 0},
    {'K', "Kocaeli",  500, 5, 50, 10, 100, 0},
    {'F', "Fýstýklý",  500, 5, 50, 10, 100, 0},
    {'B', "Bursa",    500, 5, 50, 10, 100, 0},
    {'\0', "",        0,   0,  0,  0, 100, 0} // sonlandýrýcý (gerekirse)
};

void guncelleKentler() {
    for (int i = 0; i < KENT_SAYISI; i++) {
        Kent* k = &kentler[i];

        if (k->bizimMi) {
            int gelir = (k->aylikGelir * k->vergi) / 100;
            k->butce += gelir;
        } else {
            k->vergi = rand() % 26; // 0-25 arasý yeni vergi oraný

            if (rand() % 2 == 0 && k->butce >= 250) {
                k->asker+=4;
                k->butce -= 100;
            }

            int gelir = (k->aylikGelir * k->vergi) / 100;
            k->butce += gelir;
        }

        // Memnuniyet vergiyle azalýr
        k->memnun -= 0.1 * k->vergi;
        if (k->memnun < 0) k->memnun = 0;
        if (k->memnun > 100) k->memnun = 100;

        // Asker kaybý: memnuniyetsizlik oranýna göre
        int memnuniyetsizlik = 100 - k->memnun;
        int sans = rand() % 100;
        if (sans < memnuniyetsizlik && k->asker > 0) {
            k->asker--;
        }
    }
}



void harita(char matris[SATIR][SUTUN]) {
    system("cls"); // Windows için ekran temizle
    for (int i = 0; i < SATIR; i++) {
        for (int j = 0; j < SUTUN - 1; j++) { // -1 çünkü \0 var
            putchar(matris[i][j]);
        }
        putchar('\n');
    }
}

void kentSec(char kent, char kontrolEdilenKent){
    system("cls");

    printf("====== KENT BÝLGÝSÝ ======\n");

    int bulundu = 0;
    for (int i = 0; i < KENT_SAYISI; i++) {
        if (kentler[i].harf == kent) {
            bulundu = 1;
            printf("Þehir: %s\n", kentler[i].isim);
            printf("Bütçe: %d\n", kentler[i].butce);
            printf("Vergi Oraný: %.3f%%\n", kentler[i].vergi);
            printf("Aylýk Gelir: %d\n", kentler[i].aylikGelir);
            printf("Asker Sayýsý: %d\n", kentler[i].asker);
            printf("Memnuniyet Oraný: %.3f%%\n", kentler[i].memnun);

            if (kentler[i].bizimMi) {
                printf("\n>> Bu kent sizin kontrolünüzde.\n");

                // Geliþtirilebilir: oyuncuya seçenekler sunabilirsiniz
                printf("\nYönetim Seçenekleri:\n");
                printf("1. Vergi oranýný deðiþtir\n");
                printf("2. Asker üret (25 birim maliyet)\n");
                printf("3. Savaþ aç\n");
                printf("Seçiminiz (yoksa bir tuþa basýn): ");

                char secim = getch();
                if (secim == '1') {
                    printf("\nYeni vergi oranýný girin (0-100): ");
                    int yeniVergi;
                    scanf("%d", &yeniVergi);
                    if (yeniVergi >= 0 && yeniVergi <= 100) {
                        kentler[i].vergi = yeniVergi;
                        printf("Vergi oraný güncellendi.\n");
                    } else {
                        printf("Geçersiz oran!\n");
                    }
                }
                else if (secim == '2') {
                    if (kentler[i].butce >= 25) {
                        kentler[i].asker += 1;
                        kentler[i].butce -= 25;
                        printf("Asker üretildi.\n");
                    } else {
                        printf("Yetersiz bütçe.\n");
                    }
                }
                else if (secim == '3') {
                    printf("\nHangi kente savaþ açmak istiyorsunuz? Harfini giriniz: ");
                    char hedefHarf;
                    scanf(" %c", &hedefHarf);

                    int hedefIndex = -1;
                    int bizimIndex = -1;

                    for (int i = 0; i < KENT_SAYISI; i++) {
                        if (kentler[i].harf == hedefHarf) hedefIndex = i;
                        if (kentler[i].harf == kent) bizimIndex = i;
                    }

                    if (hedefIndex == -1 || hedefIndex == bizimIndex) {
                        printf("Geçersiz þehir.\n");
                    } else if (kentler[hedefIndex].bizimMi) {
                        printf("Bu kent zaten sizin kontrolünüzde.\n");
                    } else {
                        int a = kentler[bizimIndex].asker;
                        int d = kentler[hedefIndex].asker;

                        if (a == 0) {
                            printf("Hiç askeriniz yok! Savaþ açamazsýnýz.\n");
                        } else {
                            float sans = (float)a / (a + d) * 100;
                            int r = rand() % 100;

                            printf("\nSavaþ baþladý! Kazanma þansýnýz: %.1f%%\n", sans);
                            if (r < sans) {
                                printf("Tebrikler! %s kentini ele geçirdiniz.\n", kentler[hedefIndex].isim);
                                kentler[hedefIndex].bizimMi = 1;
                                kentler[hedefIndex].asker = a / 2; // kalan askerle baþlat
                                kentler[bizimIndex].asker = a / 2;
                            } else {
                                printf("Savaþý kaybettiniz. Askerlerinizin tamamý yok oldu.\n");
                                kentler[bizimIndex].asker = 0;
                            }
                        }
                    }

                    printf("\nDevam etmek için bir tuþa basýn...");
                    getch();
                }


            } else {
                printf("\n>> Bu kent sizin kontrolünüzde deðil.\n");
            }
            break;
        }
    }

    if (!bulundu) {
        printf("Bilinmeyen þehir: %c\n", kent);
    }

    printf("\nDevam etmek için bir tuþa basýn...");
    getch();
}



int main() {
    setlocale(LC_ALL, "Turkish");
    srand(time(NULL));  // rand için
    time_t sonGuncelleme = time(NULL);
    char kontrolEdilenKent = '\0';  // henüz yönetilen kent yok

    char map[SATIR][SUTUN] = {
        "                                           ========   ====         ==========       ",
        "                                                     =                ===  =         ",
        "                                                  ======         ============        ",
        "                                           ==========A======K================        ",
        "                                       ======================================        ",
        "                   ======Ç========Y==========================================        ",
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

    printf("Karakterinizin adýný giriniz (tek karakter): ");
    char karakter = getchar();

    // Baþlangýç pozisyonu, örneðin 5.satýr 29.sütun (haritadaki '=' karakterlerinden biri)
    int x = 5, y = 29;
    map[x][y] = karakter;

    while(1){
        // ZAMAN KONTROLÜ BURADA
        time_t simdi = time(NULL);
        if (difftime(simdi, sonGuncelleme) >= 5.0) {
            guncelleKentler();
            sonGuncelleme = simdi;
        }

        // Kent harflerini sabitle
        map[3][53] = 'A';
        map[3][60] = 'K';
        map[5][25] = 'Ç';
        map[5][34] = 'Y';
        map[7][31] = 'S';
        map[7][36] = 'E';
        map[8][27] = 'T';
        map[12][12] = 'F';
        map[13][37] = 'B';

        harita(map);
        printf("Hareket etmek için WASD tuþlarýný kullanýnýz (Çýkmak için q): ");

        char yon = getch();

        if (yon == 'q' || yon == 'Q') break;

        // Yeni pozisyon için geçici deðiþkenler
        int nx = x;
        int ny = y;

        switch(yon){
            case 'w': case 'W':
                nx = x - 1; // yukarý
                break;
            case 's': case 'S':
                nx = x + 1; // aþaðý
                break;
            case 'a': case 'A':
                ny = y - 1; // sola
                break;
            case 'd': case 'D':
                ny = y + 1; // saða
                break;
            default:
                continue; // geçersiz tuþsa döngüye devam et
        }

        // Harita sýnýrlarý içinde mi?
        if(nx < 0 || nx >= SATIR || ny < 0 || ny >= SUTUN -1) continue;

        // Yeni pozisyonda karakter hareket edilebilir mi kontrolü
        // Mesela sadece '=' karakteri üzerinden hareket edilsin
        if(map[nx][ny] == '=') {
            // Eski pozisyonu '=' yap
            map[x][y] = '=';

            // Yeni pozisyona karakteri koy
            map[nx][ny] = karakter;

            // Koordinatlarý güncelle
            x = nx;
            y = ny;
        }
        else if(map[nx][ny] != '=' && map[nx][ny] != ' '){
                char hedef = map[nx][ny];
            // Eski pozisyonu '=' yap
            map[x][y] = '=';

            // Koordinatlarý güncelle
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

    printf("Oyun sonlandýrýldý.\n");
    return 0;
}
