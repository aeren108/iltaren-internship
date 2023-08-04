# Time Complexity Analysis

<center>

|      **Heuristic**      | **Worst Case** | **City Count <br>From Ankara** | **City Count <br>From Ýstanbul** | **City Count <br>From Ýzmir** |
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

|      **Heuristic**      | **Worst Case** | **City Count <br>From Ankara** | **City Count <br>From Ýstanbul** | **City Count <br>From Ýzmir** |
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

## Lokal Optimum Seçimi

Ýlk ilden baþladýktan sonra, her bir komþu il için bir skor hesaplanmakta ve en yüksek skora sahip olan il seçilerek devam edilmektedir. Bu iþlem gidilecek hiç bir il kalmayýncaya kadar devam eder.

### First Order Neighbours
Bir ilin skoru, o ilin komþu sayýsý kadardýr.

### Second Order Neighbours
Bir ilin skoru, o ilin komþularýndan en fazla komþu içeren ilin komþu sayýsý kadardýr.

### Third Order Neighbours
Bir ilin skoru, o ilin komþularýndan en fazla ikinci dereceden komþu içeren ilin ikinci dereceden komþu sayýsý kadardýr.

### Closeness Centrality
Bir ilin skoru, diðer tüm illere olan en kýsa mesafelerinin toplamýyla ters orantýlýdýr.

### Betweennes Centrality
Bir ilin skoru, grafta bulunan tüm illerin birbirine olan en kýsa yollarýnda kaç kere bulunduðudur. Diðer bir deðiþle bir node'un grafta ne kadar merkezde yer aldýðýnýn bir ölçüsüdür. Örnek: Sadece 3 ilin (Ankara, Ýstanbul, Bolu) olduðu bir grafta Ankara'dan Ýstanbul'a en kýsa yol Bolu'dan geçiyor, Bolu'nun skoru bu yüzden 1'dir. Ankara ve Ýstanbul diðer herhangi iki ilin arasýndaki en kýsa yolda yer almadýðýndan Ankara ve Ýstanbul'un skoru 0'dýr.
<br> (https://en.wikipedia.org/wiki/Betweenness_centrality)

### Distance To Previous
Bir ilin skoru, önceden ziyaret edilmiþ illere olan en kýsa mesaflerin toplamýyla ters orantýlýdýr.

### Distance To Start
Bir ilin skoru, baþlangýç iline olan en kýsa mesafeyle ters orantýlýdýr.

### Linear Combination
Bir ilin skoru, **First Order Neighbours**, **Third Order Neighbours**, **Closeness Centrality** ve **Distance To Start** fonksiyonlarýnýn aðýrlýklý toplamýdýr. Þu ana kadar en optimum sonuç veren aðýrlýklar sýrasýyla: `[-0.1, 0.2, 0.5, 0.1]`.

#### Aðýrlýklarýn ve Fonksiyonlarýn Seçimi

- Bir node'un direkt komþusu en fazla olan komþusunu ziyaret etmek, diðer az komþusu olan node'lara gelecekte ziyaret etmeyi zorlaþtýrýyor. Çünkü zaten az baðlantýlarý olduðundan graf üzerinde ilerledikçe o node'lara baðlý olan bir node'da bulunma olasýlýðý hayli az. Bu fonksiyonun aðýrlýðý negatif olacak þekilde kullanýlabilir.

- Heuristic fonksiyonlarýn lineer kombinasyonu düþünüldüðünde bu fonksiyonlarýn çýktýlarýnýn ayný aralýkta olmasý aðýrlýklarý belirleme açýsýndan kolaylýk saðlayacaktýr.

- Lokal optimum seçmek için diðer bir yaklaþým da baþlangýç noktasýna uzaklýðý en az olan node'u seçmek olabilir. Böylece baþlangýç noktasýndan en az miktarda uzaklaþarak gelecekte daha fazla uzak node ziyaret edilebilir.

## Diðer Denenebilecek Yöntemler

- Grafýn anlýk durumuna ve özelliklerine göre (edge, vertex sayýsý gibi ?) her adýmda farklý bir fonksiyon seçilebilir.
- Her adýmda farklý fonksiyon seçimi stokastik de olabilir. Markov process gibi ?

