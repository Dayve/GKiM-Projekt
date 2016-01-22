### Projekt z przedmiotu "Grafika Komputerowa i Multimedia"

**Temat:**
  1. Pierwsza część: (SKOŃCZONA)
    + Pierwszy program: zapis wartości pixeli z obrazka w formacie BMP do pliku binarnego, 5 bitów na kanał.
    + Drugi program: odczyt z wygenerowanego pliku binarnego i eksport BMP.
  2. Druga część: (SKOŃCZONA)
    + Pierwszy program: kodowanie arytmetyczne lub Byterun na zawartości pliku binarnego.
    + Drugi program: odkodowanie pliku, odczyt i eksport BMP.

**Narzędzia potrzebne do kompilacji:**
+ [SFML](http://www.sfml-dev.org/index.php) - biblioteka graficzna dla języka C++.

Opcjonalnie:
+ [GNU Make](https://www.gnu.org/software/make/) - narzędzie wspomagające kompilację, tutaj jedynie pośrednie.
+ [CMake](https://cmake.org/) - narzędzie do tworzenia m.in. plików makefile (na których działa GNU Make). Automatycznie generuje zależności.
<br/>Lub:
+ Dowolne IDE, np: [Code::Blocks](http://www.codeblocks.org/), [Eclipse IDE](https://eclipse.org/)

**Kompilacja projektu:**
+ **CMake + Terminal:**
  1. Jeśli korzystamy z systemu Windows, to zmieniamy ścieżkę do biblioteki SFML w pliku `CMake_config/ConfigSFML.cmake`, [linijka 4](https://github.com/Dayve/GKiM-Projekt/blob/master/CMake_config/ConfigSFML.cmake#L4). Dodatkowo należy wziąć pod uwagę różnice w zapisie ścieżek, konkretnie używanie `/` oraz `\`
  2. Przechodzimy do katalogu "bin" w danym podkatalogu ("BMP to File" lub "File to BMP").
  3. Uruchamiamy CMake, żeby wygenerować makefile: `cmake ../src`.
  4. Następnie kompilujemy kod: `make`. <br/>
  (Kroki 2, 3, 4 wykonujemy dla obu części projektu)
  5. I możemy już uruchomić aplikację: <br/>
  `./btf` (**B**MP **t**o **F**ile, program zamieniającego BMP na plik binarny), <br/>
  lub <br/>
  `./ftb` (**F**ile **t**o **B**MP, program zamieniającego plik binarny na BMP). <br/>
  Jeśli nie zapewnimy parametrów (czyli uruchomimy program powyższym poleceniem) to wyświetli on prawidłowy format komendy i zakończy działanie.

+ **IDE - na przykładzie Code::Blocks:**
  1. Pobieramy pliki projektu (np. klikając "Download ZIP" na stronie głównej projektu)
  2. Tworzymy nowe projekty w obu podkatalogach ("BMP to File" oraz "File to BMP") i konfigurujemy je (oba) odpowiednio:
    * Konfigurujemy linkowanie SFMLa (np: według [tego tutoriala](http://cpp0x.pl/kursy/Kurs-SFML-2-x-C++/Instalacja-i-konfiguracja/Konfiguracja-SFML-2-0-Code-Blocks/464))
    * W "Project"->"Build Options" zaznaczamy "Have g++ follow (...) -std=c++11"
    * Dodajemy pliki projektu: "Project"->"Add files recursively" i wybieramy "src" z tego samego folderu oraz "Shared Code"
    * Zmieniamy odpowiednio argumenty (normalnie podawane z linii komend) w "Project"->"Set programs' arguments"
    * Możemy też (opcjonalnie) zmienić folder na wygenerowane pliki w "Project"->"Properties"->"Build Targets"

**Uruchamianie programów dla danych plików:**
+ **CMake + Terminal:**
  + Dla programu **btf** podajemy:
    - Jako pierwszy argument ścieżkę do pliku BMP (z nazwą i rozszerzeniem .bmp)
    - Jako drugi argument 0-2 dla rodzaju kodowania:
      + 0 kodowanie arytmetyczne na 5 bitach
      + 1 kodowanie Byterun
      + 2 skalowanie wartości na 5-bitowe (z zapisem jedna obok drugiej)
    - Jako trzeci argument 0-1 dla opcji skala szarości:
      + 0 z zachowaniem kolorów
      + 1 przejście na odcienie szarości

    W bieżącym układzie folderów będzie to np: `./btf ../data/small.bmp 1 0`. 
    
  + Żeby przetestować poprawność działania dla bieżącego układu folderów musimy skopiować wygenerowany przez **btf** plik: <br/>`cp BMP\ to\ File/data/output.file File\ to\ BMP/data/`<br/>
  Jeśli nie chcielibyśmy tego robić to oczywiście możemy zmienić [ścieżkę](https://github.com/Dayve/GKiM-Projekt/blob/master/BMP%20to%20File/src/ImageWrapper/ImageWrapper.cpp#L26), pod którą program zapisuje plik "output.file". Podobnie możemy zmienić [ścieżkę](https://github.com/Dayve/GKiM-Projekt/blob/master/File%20to%20BMP/src/FileWrapper/FileWrapper.cpp#L22), pod którą jest zapisywany generowany plik BMP.
  
  + Dla programu **ftb** podajemy:
    - Jako jedyny argument ścieżkę do pliku binarnego (z nazwą i rozszerzeniem .file)
  
    W bieżącym układzie folderów będzie to np: `./ftb ../data/output.file`.

+ **IDE - na przykładzie Code::Blocks:**
Jeśli zmieniliśmy odpowiednio argumenty (normalnie podawane z linii komend) w "Project"->"Set programs' arguments" to zostaną one podane automatycznie




