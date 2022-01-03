# std::vector
------------------------

Vector container'ı bir dinamik dizidir. Yani, öğeler contigues şekilde dinamik olarak allocate edilmiş bir bellek bloğunda tutuluyorlar.
Bu bellek bloğunda tutulan fiziksel olarak öğe sayısı, containerın ```size```'ı.
Allocate edilmiş bellek bloğunda yeni bir bellek alanına ihtiyaç duymadan tutulabilecek öğe sayısı vectorun ```capacity```'si.
Temel Avantajı sondan ekleme amorties constant time(ortalama olarak sabit zamanda) index ile erişim constant time'dır
En dikkat edilmesi gereken nokta, size capacity ye eşit olduğunda bir insert işlemi yapılırsa, kapasite yetersiz olduğundan reallocation olacak, reallocation yeni allocate edilmiş bir bellek alanına taşıyacak. Eski bellek bloğundaki öğeler yenisine kopyalanacaklar yada taşınacaklar.
Productionda container ihtiyacımızın en az %70 ini karşılayan sınıf bu. Vectorun önemli bir avantajı. Dinamik dizi veri yapısının cache friendly olması. Bellekte ardışık oldukları için l2 - l3 cache e belli bir miktarını alabiliriz. Burada cache hit cache miss oranı çok daha iyi. Günümüzde en çok performansı etkileyen veri yapısı bu. Ortalama performansı en yüksek veri yapısı vector.
Özellikle çok fazla öğe tutulacaksa, sondan değilde herhangibir noktadan ekleme silme yapılacaksa, belirli bir sıra ilişkisiyle elemanları tutacaksa başka alternatiflere bakmak lazım. Tipik olarak ekleme sondan ve index erişimi varsa vectörden iyisi yok.
```c++
template <typename T, typename A = std::allocator<T>>
class Vector{
};
```
```c++
int main()
{
	vector<int>ivec;
	ivec.size = 0 
	ivec.empty() true değer döndürecek.
	ivec.capacity() vectorun kapasitesini döndürüyor. türü size_type yani size_t ile aynı.
	vector<int>::size_type n = ivec.size();
	auto n = ivec.size();
	std::size_type  = ivec.size();
	vector<ivec>(10);
	ivec.push_back(2); //size = 11
	ivec.push_back(3); //size = 12
	ivec.push_back(7); //size = 13
	std::cout << "size = " << ivec.size() << "\n";
	std::cout << "cap = " << ivec.capacity() << "\n";
	ivec.push_back(5); //size = 14
	ivec.push_back(7); //size = 15. size capacity ile aynı oldu
	ivec.push_back(34); //Burada reallocation olacak artık. size16 capacity 22 oldu.
	std::cout << "size = " << ivec.size() << "\n"; 
	std::cout << "cap = " << ivec.capacity() << "\n"; kapasite artı. Kapasite artma miktarı implementation defined.
}
```
	
Vectorde iki şeye dikkat edilmeli 
 - Maliyet - Gereksiz Reallocation
 - Iterator Invalidation 
 
vectorun öğelerini gösteren pointerlar veya referanslar olabilir. Herhangi bir eklemede reallocation yapıldığında yeni bellek alanına kopyalandığında,
eski bellek alanını gösteren pointerlar dangling hale gelir.Buna çok dikkat edilmeli.

```reserve``` kapasiteyi kesin azaltır denemez.
Mesela capacity = 100'000 ve size = 5'000  olsun
svec.reserve(5000) dersek burada fazla kapasiteyi daraltır diye birşey yok. ```Nonbinding shrink request```. ```shrink_to_fit()``` C++11 ile geldi ve bu işi yapıyor.

Vectörde kapasite otomatik büyür ama kapasite küçültülmez.

```c++
int main()
{
	vector<int>ivec(100000);
	for(int i = 0; i < 100000;++i)
	{
		ivec.push_back(i);
	}
	std::cout << ivec.capacity() << "\n";
	ivec.clear();
	std::cout << ivec.capacity() << "\n"; //Kapasite hep aynı
}
```

NOT : move ctor noexcept garantisi verirse reallocation sirasinda move ctor ile üyeleri yeni bellek alaninda oluşturuyor 

```c++
// shrink_to_fit
int main()
{
	vector<int>vec(672334);
	
	std::cout << vec.size << " " << vec.capacity() << "\n"; // size  672334 capacity 672334
	
	vec.erase(vec.begin() + 5, vec.end());
	
	std::cout << vec.size << " " << vec.capacity() << "\n"; // size 5 capacity 672334
	vec.shrink_to_fit();
	std::cout << vec.size << " " << vec.capacity() << "\n"; // size 5 capacity 5	
}
```

## Constructors
 - [Default Constructor](#default-constructor)
 - [Copy Constructor](#copy-constructor)
 - [Move Constructor](#move-constructor)
 - [Size Type Constructor](#size-type-constructor)
 - [Initializer List Constructor](#initializer-list-constructor)
 - [Range Constructor](#range-constructor)
 

### Default Constructor
```c++
vector<int> x; // default init 
vector<int> y{}; // value init Default ctor çağrılacak ikisinde de.
// Size 0 olacak empty true döndürecek.
if(x.empty()) // boş olduğunu sınıyoruz.  x.size() == 0 yazmıyoruz
if(empty(x)) //te yazılabilir.
```

### Copy Constructor
```c++
int main()
{
	vector<int> x(6547); // Containerın size 6547
	vector<int> y = x;
	vector<int> y(x);   
	vector<int> y{x};
	auto y{x};			//bunlarda çağrılan hep copy ctor.
}
```

### Move Constructor

Argüman olarak rvalue expression gönderirsek Move Ctor çağrılacak.
Vector implementasyonda small buffer optimization yok ama Boost library de var.
Kopyalama semantiği deep copy, move semantik ise pointerları kopyalıyor sadece. 3 pointer var vectorde. vector başlangıç adresi, push back ile ekleme yapılacak adres, diğeride sonunu tutuyor.
	
**moved-from object :**
Kaynağı Çalınmış nesne halen geçerli bir nesnedir.(kullanılabilir ama değeri konusunda bir öngörüde bulunulamaz)
Taşınmış bir nesne için dtor çağrıldığında herhangibir UB veya Resoure leak oluşmayacak
Taşınış nesneye yeni değer atamakta problem değil.
```c++
Dclass x;
Dclass y(std::move(x));
y = z; //yapılabilir.
```
Mümkün olduğunca taşınmış nesneleri kullanmamak gerek. İllaha kullanılacaksa belirli bir değere sahip olduğuna varsayamayız.
Bu sınıf doğru yazılmışsa, sınıfın değişmezleri(invariantları) korunacak. Mesela nesne boş ise size func 0 döndürür.

### Size Type Constructor
Size type parametreli bir ctor var. bu vectorü n tane öğe ile başlatiyor.
```c++
int main()
{
	size_t n;
	std::cout << "kac tarih\n";
	cin>> n;
	vector<Date>dvec(n); // Bu Ctorda n, size değeri olacak. Bu ctor öğeleri default init ediyor. 
	std::cout << dvec.size()<< "\n";
}
```

Bu constructor value init ediyor. ```vector<int>dvc(10); //tüm elemanlar 0 olma garantisinde.``` Yani sınıf türden nesnelerde default ctor çağrılacak, bool türü için false, pointer ise nullptr olacak.


### Initializer List constructor 
Normalde uniform initialization parantez ile ayni anlama geliyor.
Burada sınıfın initializer list ctoru olduğundan küme parantezi ile normal parantez farklı anlama geliyor.
```c++
// Küme parantezi kullanırsak o initializer list constructor demek.
vector<int>x(10); // size_t parametreli ctor kullanıldı. X in size 10 ve öğeler 0
std::cout << "x.size() = "<< x.size() << "\n"; // 10
print(x); // 0 0 0 
vector<int>x{10}); // size 1 oldu ve değeri 10
std::cout << "x.size() = "<< x.size() << "\n"; // 1
print(x); // 10 
//vector<size_type n, const T&val>; Örnek aşağıda
vector<int>ivec(100,5); // 100 tane 5
vector<int>ivec{100,5}; // 100 ve 5 değerleri tutulacak size değeri 2
```

### Range Constructor
```c++
vector<int>vx;
vector<double>vy = vx; // vector<int> ve vector<double> farklı sınıflar atama yapılamıyor. İsteselerdi yaparlardı member template ile ama tercih edilmemiş.
vy = vx ; // Bu da aynı şekilde hata.
```
```c++
int main()
{
	vector<int> myvec;
	list<int>mylist(myvec); // SENTAKS HATASI
}
```
Range Constructor iterator parametreli ve iteratörlerin türünün aynı olması gerekmiyor. Önemli olan iteratör konumundaki öğelerin birbirine atanabilmesi gerekiyor
```c++
vector<int> mycvec{1,2,3,4,6,8,7,5,3};
vector<double> dvec(myvec.begin(),myvec.end()); // GEÇERLI çünkü intten double a geçerli dönüşüm var.
vector <const char *>vec {"Ali","Can","Oya"};
list<string>mylist{vec.begin(), vec.end()}; // GEÇERLİ.const char * dan stringe dönüşüm var.
	
//Range constructor çok önemli. Farklı containerları içindeki öğeler birbirine assignable ise başka bir containerla başlatabliyoruz.
int a[]{1,4,6,8,9};
vector<int>ivec(begin(a),end(a));	//GEÇERLİ
vector ivec(begin(a),end(a));		//CTAD ÇALIŞIYOR BURADA.SEBEBİ DEDUCTION GUIDE BAŞLIKLI SENTAKS ÖZELLİĞİ.
```

Constructor Table :
```c++
vector<Elem> c;			//Default constructor; creates an empty vector without any elements
vector<Elem> c(c2)İ		// Copy constructor; creates a new vector as a copy of c2 (allelements are copied)
vector<Elem> c = c2;		// Copy constructor; creates a new vector as a copy of c2 (allelements are copied)
vector<Elem> c(rv);		// Move constructor; creates a new vector, taking the contentsof the rvalue rv (since C++11)
vector<Elem> c = rv;		// Move constructor; creates a new vector, taking the contentsof the rvalue rv (since C++11)
vector<Elem> c(n);		// Creates a vector with n elements created by the default constructor
vector<Elem> c(n,elem);		// Creates a vector initialized with n copies of element elem
vector<Elem> c(beg,end);	// Creates a vector initialized with the elements of the range[beg,end)
vector<Elem> c(initlist);	// Creates a vector initialized with the elements of initializer list initlist (since C++11)
vector<Elem> c = initlist;	// Creates a vector initialized with the elements of initializerlist initlist (since C++11)
c.~vector();                    // Destroys all elements and frees the memory
```

### Vector üye fonskiyonları
- size() : Container'da tutulan öğe sayısı
- capacity(): Toplam allocate edilen bellek bloğu
- max_type() (tüm containerlarda ortak bir func)
- empty() : Container boş mu dolu mu
- begin()
- cbegin()
- rbegin()
- crbegin()
- end()
- cend()
- rend()
- cren()

### Öğelere erişmek
operator[] functionu referans döndürüyor. const vector için const referans döndürüyor.
```c++
int main()
{
	vector<int>ivec{1,2,4,6,8,9,5,4};
	for(size_t i = 0; i < ivec.size();++i)
	{
		cout << ivec[i] <<"\n";
		cout << ivec.operator[](i) <<"\n"; // ikiside aynı.
		
		++ivec[i]; // yapılabilir
	}
}
```

[] operator func hata durumunda exception throw etmiyor.
```c++
int main()
{
	vector<int>ivec{1,2,4,6,8,9,5,4};
	
	try{
		auto val = ivec[54]; // undefined behavior
		auto val = ivec.at(54); // burada exception throw edilir ve yakalanir
	}
	catch (const std::exception &ex){
		std::cout << "Exception cought : " << ex.what() << "\n"; 
	}
}
```

### front ve back 
```front()``` functionu sequence containerlarda ilk eğeye referans döndürüyor. ```back()``` functionu sequence containerlarda son eğeye referans döndürüyor
```c++
int main()
{
	vector<string>svec{"Ali","Ayse","Hasan","Selo"};
	//svec[0]
	//svec.front() //ikiside aynı ama  front template ise tüm sequence containerlar için geçerli olacak.
	svec.front() += "han";
	svec[svec.size() - 1];
	svec.back() += "naz";
	print(svec);	// "Alihan","Ayse","Hasan","Selonaz"
}
```

Container const ise yukarıdaki tüm işlemlerin return değeri const & olacak.
```c++
const vector<string>svec{"Ali","Ayse","Hasan","Selo"};
auto x = svec[0]; //GEÇERLI.
svec[0] = "kerim"; //SENTAKS HATASI
// Aynı durum diğerleri içinde geçerli.
```

### Bir vectoru dolaşmak
```c++
int main()
{
	vector<string>svec{"Ali","Ayse","Hasan","Selo"};
	for(size_t i{0}; i < svec.size(); ++i)
	{
		std::cout << svec[i] << "\n"; 
		// std::cout << svec.at(i) << "\n";  Buda kullanılabilir.
	}

	//set amaçlı
	vector<string>svec{"Ali","Ayse","Hasan","Selo"};
	for(auto iter = svec.begin(); iter != svec.end(); iter++)
	{
		*iter += "can";
	}
	
	//get amaçlı
	vector<string>svec{"Ali","Ayse","Hasan","Selo"};
	for(auto iter = svec.cbegin(); iter != svec.cend(); iter++)
	{
		std::cout << iter->length() << " ";
	}
	
	//Birçok durumda range base for loop kullanılabilir
	vector<string>svec{"Ali","Ayse","Hasan","Selo"};
	for(const auto &s = svec) // string &s : svec te yazılabilir.
	{
		std::cout << s << "\n";
	}
	
	for(auto&&s = svec) // burası sağ taraf değil forwarding referans. Çıkarım yapılıyor.
	{
		//
	}
}
```

### resize
reserve ile resize karıştırılmamalı.
```c++
int main()
{
	vector<string>svec{"Ali","Ayse","Hasan","Selo"};
	std::cout << "size = " << svec.size() << "\n";
	svec.resize(40);
	std::cout << "size = " << svec.size() << "\n"; 
	// ilk 4 öğe belli ama diğer öğeler eğer bu sınıf türündense default init ediliyor
	// aritmetik türden değerler tutan vector olsaydı, beklenen öğeler 0, pointer ise nullptr.
	svec.resize(2);
	std::cout << "size = " << svec.size() << "\n"; 
	// ilk iki öğe kaldı ve size 2 oldu.
}
```

resize in bir overload u daha var. ekleme yapildiğinda eklenecek değerlerin hangi değerlerle başlatilacağini belirtiyor.
```c++
int main()
{
	vector<int> ivec{1,2,3,4,5,6,7};
	ivec.resize(20,-1); // 1,2,3,4,5,6,7,-1,-1,-1,-1 .... -1 yapar
}
```

### sequence containerlarin insert ve erase functionlari
erase in 2 biçimi var. 
1. iterator alıyor ve konumdaki değeri siliyor
2. range alıyor ve rangedekileri siliyor.
```c++
int main()
{
	vector<string>svec{"Ali","Ayse","Hasan","Selo"};
	svec.erase(svec.begin()); // ilk konumdaki öğeyi siler
	svec.erase(svec.begin() + 1); // ikinciyi siler
	//list sınıfında bu böyle bir toplama işlemi olmayacak.bu sebeple next kullanacağız.
	svec.erase(next(svec.begin())); // beginin bir sonrasını silecek yani 2. öğe
	// Son öğe
	svec.erase(svec.end() - 1);
	svec.erase(prev(svec.end()));
	// silme işleminde diğer elemanlar kaydırılıyor.
	svec.pop_back(); // son elemanı siliyor ama return değeri yok. dezavantajı bu.
	// silme functionlarının return değeri var. Bu çok önemli.
	auto iter = svec.erase(svec.begin()); // silinen öğeden sonraki konumu tutuyor.
	std::cout << *iter << "\n";
}
```
range parametreli erase func
```c++
int main()
{
	vector<string>svec{"Ali","Ayse","Hasan","Selo"};
	svec.erase(svec.begin() +1, svec.end() -1 ); // ilk v son hariç hepsini siler.
	svec.erase(svec.begin(), svec.end() ); // hepsini siler
	svec.clear(); // buda siler hepsini ve bu tercih edilir yukarıdakine göre.
}
```

### Atama işlemleri
```
c = c2				Assigns all elements of c2 to c
c = rv				Move assigns all elements of the rvalue rv to c (since C++11)
c = initlist			Assigns all elements of the initializer list initlist to c (since C++11)
c.assign(n,elem)		Assigns n copies of element elem
c.assign(beg,end)		Assigns the elements of the range [beg,end)
c.assign(initlist)		Assigns all the elements of the initializer list initlist
c1.swap(c2)			Swaps the data of c1 and c2
swap(c1,c2)			Swaps the data of c1 and c2
```

Bir vectöre başlka bir vectorü kopaylayarak taşıyarak yada init list atayabiliriz.
bunlar atama operator funcları.
```c++
int main()
{
	vector<string>svec;
	vector<string>x{"ali","veli","can","nur","beyza"};
	rfill(svec,10,rname);
	print(svec);
	svec = std::move(x); // move assignment ile atama yapıldı
	svec = x; // copy assignment ile tama yapıldı
	//yazdırıp görebiliriz. print ile
}
```

initializer list atamasıda var.
```c++
int main()
{
	vector<string>svec;
	vector<string>x{"ali","veli","can","nur","beyza"};
	rfill(svec,10,rname);
	print(svec);
	svec = {"remzi","hasan",huseyin}; // buda init list ataması
}
```

range assignment
Bir vectöre farklı türden bir vectorü atayamayız.Assign func ının return değeri yok ama copy asignemnt func ının return değeri *this yine. Kendisi yani.
Ama range ctor gibi range assignment ta var. Prodda çok sık kullnılıyor.
```c++
int main()
{
	vector<string>svec;
	vector<string>x{"ali","veli","can","nur","beyza"};
	list<string>mylist{"kerim","fuat","naci"};
	svec.assign(mlist.begin(), mylist.end()); // RANGE ASSIGN. farklı containerlar olmasına ramen atama range parametreden ötürü yapılabildi.
	svec.assign{"hasan","huseyin","mert"}; // INITIALIZER LIST ASSIGNMENT
	
	print(svec);
}
```
Farklı türden ama assign burada iteratör parametreli olduğundan bu atama gerçekleşebilir. fill assign da var :
```c++
int main()
{
	vector<int>ivec;
	ivec.assign(20,3); // 20 adet 3. Assign ın return değeri yok.
	print(ivec);
}
```

### insert
Vectorde her ne kadar sondan ekleme constant time ama istediğimiz yerinden de ekleme yapabiliriz.
```
c.push_back(elem)		Appends a copy of elem at the end
c.pop_back()			Removes the last element (does not return it)
c.insert(pos,elem)		Inserts a copy of elem before iterator position pos and returns the position of the new element
c.insert(pos,n,elem)		Inserts n copies of elem before iterator position pos and returns the position of the first new element (or pos if there is no new element)
c.insert(pos,beg,end)		Inserts a copy of all elements of the range [beg,end) before iterator position pos and returns the position of the first newelement (or pos if there is no new element)
c.insert(pos,initlist)		Inserts a copy of all elements of the initializer list initlist before iterator position pos and returns the position of the first new element (or pos if there is no new element; since C++11)
c.emplace(pos,args...)		Inserts a copy of an element initialized with args before iterator position pos and returns the position of the new element (since C++11)
c.emplace_back(args...) 	Appends a copy of an element initialized with args at the end (returns nothing; since C++11)
c.erase(pos)			Removes the element at iterator position pos and returns the position of the next element
c.erase(beg,end)		Removes all elements of the range [beg,end) and returns the position of the next element
c.resize(num)			Changes the number of elements to num (if size() grows new elements are created by their default constructor)
c.resize(num,elem)		Changes the number of elements to num (if size() grows newelements are copies of elem)
c.clear()			Removes all elements (empties the container)
```
```c++
int main()
{
	vector<string>svec{"nur","can","demir","avni"};
	print(svec);
	
	svec.insert(svec.begin(),"Ali"); // başlangıca Ali eklendi
	print(svec);
	
	svec.insert(next(svec.begin()),"Ali"); // baştan 2. konuma ali eklendi
	print(svec);
	
	//Nereye ekleme yaparsak o konuma eklenir ve sonrası kaydırılıyor.
	auto iter = svec.insert(next(svec.begin()),"Ali"); 
	cout << *iter <<"\n"; // Eklenen öğenin konumunu döndürür.Yani Ali.
}
```

### her turda başa ekleme yapan kod
```c++
int main()
{
	vector<string>svec{"nur","can","demir","avni","sevda"};
	vector<string> destvec;
	for(const auto & s : svec)
	{
		destvec.insert(destvec.begin(),s); // hangi konumu verirsek oraya ekler ve eklediği öğeye referans döndürür.
		print(svec);
		getchar();
	}
} 
```

eklemeyi range olarak yapabiliriz:
```c++
int main()
{
	int a[] = {2,3,5,7,11};
	vector<int> ivec {10,20,30,40,50};
	print(ivec);
	ivec.insert(ivec.begin() + 2 , begin(a),end(a)); // begin +2 konumuna, bir range deki öğeleri insert etti.
	print(ivec);
}
```
## container emplace functionlari
Verim için çok önemli. Heapte allocate edilen bir alanımız var olsun. 3 adet eleman var. yani size 3. Bular zaten hayatta.
Pushback veya herhangibir insert func çalıştıktan(hangisinin olduğu önemsiz) sonra bizim nesnemizin 4. eleman olaran hayata başladığını düşünelim(son konumda).
Burada 3 ihtimal var.
1. Eklenen nesne kopyalama yoluyla eklenecek yani copy ctor çağrılacak.Yani eklenen bellek bloğu this adresi olarak kullanıp sınıfın copy ctoru çağrılacak.
2. Bu nesne bir sağ taraf değeri ise yani birdaha kullanılma ihtimali yoksa ya da move kullanılarak bu gözden çıkarılmışsa o zaman bunun için move ctor çağrılacak.
   Copy ile move arasında çok büyük performans farkı olabilir
3. Emplace ne yapıyor : Sen bana ctora göndereceğinm argümanları gönder, ben containerın o nesne için kullanacağı bellek alanında doğrudan ctoru çalıştırayım.
   Yani nesneyi ilgili bellek alanında hayata getiriyor.Copy ellision gibi.
Perfect forwarding ile yapıyor bunu.
```c++
int main()
{
	vector<Date> dvec;
	Date mydate(12,4,1987);
	dvec.push_back(Date::random()); // Burada pushback in sağ taraf ref parametreli overload u çalıştı çünkü sağ taraf değeri date::random
	
	dvec.push_back(mydate); // kopyasını çıkarttı burada
	dvec.emplace_back(4,4,1944);	// Bu ise ilgili bellek alanında doğrudan nesneyi oluşturdu.Dolayısı ile containera koyacağımız nesneyi nasıl oluşturacağımız
					// baştan belliyse, insert veya push_back push_front gibi funcları çağırmak yerine, emplace funclarını çağırırsak, emplace func
					// verdiğimiz argümanları kullanarak containerda verdiğimiz alanda nesneleri doğrudan construct edecek.
					// push_back e karşı emplace_back var. 
					// push_front a karşılık emplace_front var(buarada vectorde değil list, deque de vardı push_front).
					// insert için ise doğrudan emplace var.
	
	dvec.emplace(dvec.begin(),1,1,2001); //burası emplace olduğundan konum girdik.Yukarıdaki emplace backti
	 									// nesneyi oluşturup kopyalama yerine doğrudan ilgili yerde default ctor
}
```

Tercih Sırası nasıl olmalı
1 Copy ellision (ideali bu)
2 Move
3 Copy

### resize ile hem atama hem silme yapilabilir.
```c++
int main()
{
	vector<int> ivec(100);
	ivec.erase(ivec.begin() + 16); // 16. öğe silinecek.
	ivec.resize(20); // yapılabilir. Boyutu 20 yapınca, 20 den sonrası yok artık :D
	
	size_t idx  = 34;
	ivec.erase(ivec.begin() + idx, ivec.end()); // bu şeklilde silinir. NOT : Erase return değeri var ama resize return değeri yok.
	ivec.resize(idx); // bunu yaz.size ını küçülterek silme işlemi yaptı denebilir.
	auto iter = ivec.begin() + 20;
	ivec.erase(iter,ivec.end()); // 20 den sonrasını sildik.
	ivec.resize(iter - ivec.begin()); // yine boyutunu 20 ye resize etti.
		
}
```
### Swap 
Global swap veya member swap ile swap yapılınca container elemanlarına erişir yani containerın tuttuğu pointerları takas eder.
```c++
int main()
{
	vector<int> odds{1,3,5,7,9};
	vector<int> evens{2,4,6,8,10};
	swap(odds,evens); // global swap func
	print(odds);
	print(evens);
	
	odds.swap(evens); // Buda olur aynı şey.
	evens.swap(odds); // Buda olur yine.
}
```
Bu normal takas func değil. Containerın elemanlarına yani pointerları takas ediyor.
Öğeler durduğu yerde duruyor ama pointerlar takas ediyor.

### data
Vectördeki öğeler dinamik bellek bloğunda contigues halde duruyolar. Biz dinamik bell . blok. ilk öğenin adresini
bilirsek ve dinamik dizinin size bilgisini bilirsek, bir C apisine bu adresi ve size bilgisini argüman olarak gönderebiliriz.
Bunun için ilk öğenin adresine ihtiyaç var ve bunu veren function data. Sadece data ile alırız demiyoruz tabiki.
```c++
void display_array(const int *p, size_t n)
{
	while(n--)
	{
		std::cout << *p++ << " ";
	}
	std::cout << "\n";
}
int main()
{
	vector<int> ivec{2,4,7,9,1,6};
	ivec.data(); // 2 overload u var. Hem get hem set için, int * ve const int *
	display_array(ivec.begin() .... ); // bu şekilde yazamayız çünkü const int * parametresi var ve biz argüman olarak iteratör gönderiyoruz.
	display_array(ivec.data(),ivec.size()); // bu şekilde yazılabilir.
}
```

```c++
void increment(int *p, size_t n) 
{
	while(n--)
	++ *p++; // dikkat
}
int main()
{
	vector<int> ivec{2,4,7,9,1,6};
	display_array(ivec.data(),ivec.size()); 
	increment(ivec.data(),ivec.size());
	//Sadece Data kullanılacak diye birşey yok !!!
	display_array(&ivec[0],ivec.size()); // bu şekilde yazılabilir.
	display_array(&*ivec.begin(),ivec.size()); // bu şekilde yazılabilir.
	//birsürü yazım şekli olabilir.
}
```

### ITERATOR INVALIDATION

Yeni başlayan programcıların anlamakta zorlandığı ve sılıkla hata yaptığı alanlardan biri

```c++
#include<vector>
#include<string>
#include<algorithm>
using namespace std;
int main()
{
	vector<int> svec(20);
	generate(svec.begin(), svec.end(), rname);
	copy(svec.begin(), svec.end(), ostream_iterator<string>{cout, " "});// vectordeki stringeri ıkış akımına yazdı son argümanda
	cout << "\n";
	//vectoru dolaşarak uzunluğu 5 olanları silecek, uzunluğu 6 olanlardan bir tane daha ekleyeceğiz.
	// 5 veya 6 olmayanlara dokunmayacak.
	//BU KOD ITERATOR INVALIDATIONDAN DOLAYI YANLIŞ
	for(auto iter = svec.begin(); iter != svec.end(); ++iter)
	{
		if(iter->lenght() == 5)
		{
			svec.erase(iter);
		}
		else if(iter-> size() == 6) // buralarda artık invalid olan iteratörü kullanıyoruz.
		{
			svec.insert(iter, *iter);
		}
	}
	copy(svec.begin(), svec.end(), ostream_iterator<string>{cout, " "});// Şanslı isek buradaki runtime hatasını görebiliriz. Release modda çalıştırdı hoca
	cout << "\n";
}
```
Bu soruyu yanlış yapmalarının sebebi iterator invaliadion.
Bizim iteratörlerimiz bir containerdaki bir öğenin konumunu tutuyorlar.
Mesela bizim bir iteratörümüz varsa ve bu end iteratörü değilse o zaman aslında
bir containerdaki öğenin konumunu tuyuyor.bizde bu iteratör ile öğeye erişip işlem yapabiliyoruz. Bu iteratörün geçerliliğini koruması için halen bizi bu konumdaki nesneye eriştirmesi için valid iteratör olması gerekiyor. Tıpkı C deki valid pointer gibi. Bu iterator herhangibir işlemden dolayı invalid hale gelmişse bu duruma iterator invalidation deniyor. Buna pointerlar ve referanslarda dahil. Aslen geçerli pointer / reference / iterator ün bir işlemden sonra geçersiz hale gelmesidir. Dereference etmek ub olabilir. Ub değildir ama o konumda artık olması gereken nesne değil başka bir nesne var. Containerların vector list dahil , hepsi için iteratörlerin invalid olup olmamasına yönelik kuralları var.

VECTORDEKİ KURALLAR
```
1 - Yeni boyut eski kapasiteden daha büyük olursa realloc olur. bu öğelerin biryere taşınması demek.
	yani realloc olduğunda eskilerin hepsi geçersiz hale geliyor.
2 - insert, emplace_back, emplace, pushback bu funclar çağrıldığında herhangibir realloc olmazsa
	eklemenin yapıldığı konumdan önceki nesnelere yönelik iteratörler ve referanslar geçerliliğini koruyor.
	AMA ekleme yapılan ve daha sonraki konumlar geçersiz hale geliyor.
3 - Silme işlemlerinde ise erase ve pop_back ile bir konumdaki öğe silinirse,
	bundan öncekilerin konumunu tutan pointer veya iteratörler geçerliliğini
	koruyorlar ama silinen dahil silinenden sonrakileri elemanları tutan iteratör veya pointerlar veya referanslar
	geçerliliğini kaybediyor.
```
Az önceki soruda kodu yanlış yazma sebebi tipik olarak iterator invalidation.

```c++
int main()
{
	vector<string> svec(20);
	generate(svec.begin(), svec.enc(),rname);
	print(svec);
	auto iter = svec.begin();
	// bir iteratörü silince yolumuza bir sonrakinden devam etmeliyiz zaten erase funclarıda silinenden bir sonraki öğeyi return ediyor.
	// burada iteratörün değerini değiştirip erase in return değeri yapacağız, Yani iterator silme işlemi yapunca
	// bir sonrakini gösterecek. 
	// Ekleme durumunda ise ilgili konuma eklersek, insert func ın return değeri eklenen öğeye iterator olacak. 
	// dolayısıyla insertin return değerini yine aynı iteratöre atarsam bu durumda iteratörün yeni değeri
	// eklenmiş öğenin konumu olacak. Bu durumda iteratörün değerini 2 artırmam gerekiyor. BURAYI TEKRAR DİNLE 23:00s
	
	// ikiside değilse iteratörü 1 artırıyortuz.
}
```

```c++
int main()
{
	vector<string> svec(20);
	generate(svec.begin(), svec.enc(),rname);
	print(svec);
	auto iter = svec.begin();
	while(iter != svec.end()){
		if(iter->length() == 5)
		{
			iter = svec.erase(iter); // bu durumda iteratörü artırmamak gerekiyor.Zaten erase'in return değeri bir sonraki iterator.
		}
		else if(iter -> length() == 6){
			iter = svec.insert(iter, *iter);
			iter += 2; // random access te böyle yazılır. bidirectiona olsaydı advance(iter,2) veya 2 defa artıracaktık
				   // Burada eklenen insert eklenen öğenin konumunu return eder ve aynı öğeden 2 tane var bu örnekte 2 artıracağız
		}
		else
		{
			++iter;
		}
	}
	print(svec);
}
```

Soru? 
Bir vectorden bir öğeyi sileceğiz ama silme constant time da olacak.
```c++
int main()
{
	vector<int> ivec{2,4,6,7,9,12,20,1};
	auto iter = ivec.begin() + 4; // değeri 9 olan öğe silinecek.
	iter_swap(iter, ivec.end() -1 ); // Çözüm
	ivec.pop_back();
	//2. yol iter swap ie olacak diye birşey yok.
	swap(*iter, ivec.back());
	ivec.pop_back();
}
```
Erase ile silersek karmaşıklık O(n) olacak. 
Biz O(1) istiyoruz. 
Son öğe ile yer değiştirip sonra silebiliriz.