# Time Complexity Analysis

<center>

|      **Heuristic**      | **Worst Case** | **City Count <br>From Ankara** | **City Count <br>From �stanbul** | **City Count <br>From �zmir** |
|:-----------------------:|:--------------:|:------------------------------:|:--------------------------------:|:-----------------------------:|
|  First Order Neighbours |    $O(n^3)$    |               21               |                16                |               25              |
| Second Order Neighbours |    $O(n^4)$    |               38               |                23                |               38              |
|  Third Order Neighbours |    $O(n^5)$    |               55               |                53                |               46              |
|   Closeness Centrality  |    $O(n^4)$    |               60               |                48                |               39              |
|  Betweenness Centrality |    $O(n^5)$    |               44               |                46                |               44              |
|   Distance to Previous  |    $O(n^4)$    |               14               |                18                |               22              |
|    Distance to Start    |    $O(n^3)$    |               14               |                23                |               22              |
|    Linear Combination   |    $O(n^5)$    |               76               |                55                |               50              |

**Distance**: 250 km<br>
**Tolerance**: 50 km 
</center>

<hr>

<center>

|      **Heuristic**      | **Worst Case** | **City Count <br>From Ankara** | **City Count <br>From �stanbul** | **City Count <br>From �zmir** |
|:-----------------------:|:--------------:|:------------------------------:|:--------------------------------:|:-----------------------------:|
|  First Order Neighbours |    $O(n^3)$    |               34               |                37                |               36              |
| Second Order Neighbours |    $O(n^4)$    |               50               |                50                |               40              |
|  Third Order Neighbours |    $O(n^5)$    |               51               |                58                |               58              |
|   Closeness Centrality  |    $O(n^4)$    |               26               |                55                |               53              |
|  Betweenness Centrality |    $O(n^5)$    |               54               |                46                |               56              |
|   Distance to Previous  |    $O(n^4)$    |               32               |                32                |               32              |
|    Distance to Start    |    $O(n^3)$    |               37               |                46                |               47              |
|    Linear Combination   |    $O(n^5)$    |               77               |                74                |               51              |

**Distance**: 350 km<br>
**Tolerance**: 50 km 

</center>

## Lokal Optimum Se�imi

�lk ilden ba�lad�ktan sonra, her bir kom�u il i�in bir skor hesaplanmakta ve en y�ksek skora sahip olan il se�ilerek devam edilmektedir. Bu i�lem gidilecek hi� bir il kalmay�ncaya kadar devam eder.

### First Order Neighbours
Bir ilin skoru, o ilin kom�u say�s� kadard�r.

### Second Order Neighbours
Bir ilin skoru, o ilin kom�ular�ndan en fazla kom�u i�eren ilin kom�u say�s� kadard�r.

### Third Order Neighbours
Bir ilin skoru, o ilin kom�ular�ndan en fazla ikinci dereceden kom�u i�eren ilin ikinci dereceden kom�u say�s� kadard�r.

### Closeness Centrality
Bir ilin skoru, di�er t�m illere olan en k�sa mesafelerinin toplam�yla ters orant�l�d�r.

### Betweennes Centrality
Bir ilin skoru, grafta bulunan t�m illerin birbirine olan en k�sa yollar�nda ka� kere bulundu�udur. Di�er bir de�i�le bir node'un grafta ne kadar merkezde yer ald���n�n bir �l��s�d�r. �rnek: Sadece 3 ilin (Ankara, �stanbul, Bolu) oldu�u bir grafta Ankara'dan �stanbul'a en k�sa yol Bolu'dan ge�iyor, Bolu'nun skoru bu y�zden 1'dir. Ankara ve �stanbul di�er herhangi iki ilin aras�ndaki en k�sa yolda yer almad���ndan Ankara ve �stanbul'un skoru 0'd�r.
<br> (https://en.wikipedia.org/wiki/Betweenness_centrality)

### Distance To Previous
Bir ilin skoru, �nceden ziyaret edilmi� illere olan en k�sa mesaflerin toplam�yla ters orant�l�d�r.

### Distance To Start
Bir ilin skoru, ba�lang�� iline olan en k�sa mesafeyle ters orant�l�d�r.

### Linear Combination
Bir ilin skoru, **First Order Neighbours**, **Third Order Neighbours**, **Closeness Centrality** ve **Distance To Start** fonksiyonlar�n�n a��rl�kl� toplam�d�r. �u ana kadar en optimum sonu� veren a��rl�klar s�ras�yla: `[-0.1, 0.2, 0.5, 0.1]`.

#### A��rl�klar�n ve Fonksiyonlar�n Se�imi

- Bir node'un direkt kom�usu en fazla olan kom�usunu ziyaret etmek, di�er az kom�usu olan node'lara gelecekte ziyaret etmeyi zorla�t�r�yor. ��nk� zaten az ba�lant�lar� oldu�undan graf �zerinde ilerledik�e o node'lara ba�l� olan bir node'da bulunma olas�l��� hayli az. Bu fonksiyonun a��rl��� negatif olacak �ekilde kullan�labilir.

- Heuristic fonksiyonlar�n lineer kombinasyonu d���n�ld���nde bu fonksiyonlar�n ��kt�lar�n�n ayn� aral�kta olmas� a��rl�klar� belirleme a��s�ndan kolayl�k sa�layacakt�r.

- Lokal optimum se�mek i�in di�er bir yakla��m da ba�lang�� noktas�na uzakl��� en az olan node'u se�mek olabilir. B�ylece ba�lang�� noktas�ndan en az miktarda uzakla�arak gelecekte daha fazla uzak node ziyaret edilebilir.

## Di�er Denenebilecek Y�ntemler

- Graf�n anl�k durumuna ve �zelliklerine g�re (edge, vertex say�s� gibi ?) her ad�mda farkl� bir fonksiyon se�ilebilir.
- Her ad�mda farkl� fonksiyon se�imi stokastik de olabilir. Markov process gibi ?

