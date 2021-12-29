## iteratörler

İteratörler pointer-like bir interface'e sahiptir. Dolayısı ile bunların sınıf türünden nesneler olması durumunda yine pointer gibi 
kullanılmasını sağlayan operator overloading mekanizmaları vardır. Herbir iteratörün kategorisi olmak zorundadır. Bu kategorilere göre iteratörlerin overload edilen mekanizmaları değişmektedir.

#### İteratör Kategorileri
- Output iterator
- Input iterator
- Forward iterator
- Bidirectional iterator
- Random access iterator
--------------------------------------

Bütün iteratorlerin iterator_category isimli bir nested type'ı vardır.
```c++
vector<int>::iterator::iterator_category; 
list<int>::iterator::iterator_category; 
forward_list<string>::iterator::iterator_category; 
...
```
şeklinde öğrenebiliriz. 
Buradaki nested type'lar STL deki tanımlanmış empty classlardan biri.
Bunlar:
```c++
struct output_iterator_tag;
struct input_iterator_tag;
struct forward_iterator_tag;
struct bidirectional_iterator_tag;
struct random_access_iterator_tag;

// Example
int main()
{
	cout << typeid(std::vector<int>::iterator::iterator_category).name() << "\n"; //random_access_iterator_tag s
	cout << typeid(std::list<int>::iterator::iterator_category).name() << "\n"; //bidirectional_iterator_tag
	cout << typeid(std::forward_list<int>::iterator::iterator_category).name() << "\n"; //forward_iterator_tag
	cout << typeid(std::ostream_iterator<int>::iterator_category;).name() << "\n"; //output iterator tag  
	cout << typeid(std::istream_iterator<int>::iterator_category).name() << "\n"; //istream iterator tag
}
```
| Kategori               | Operasyonlar                                                                                                                                                                                                                | Örnekler                                                                             |
|------------------------|-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|--------------------------------------------------------------------------------------|
| Output Iterator        | Copy constructible<br>++it, 	it++, =, *it, it-> (sol taraf değeri)                                                                                                                                                             | ostream_iterator<br>ostreambuf_iterator<br>                                          |
| Input Iterator         | Copy constructible<br>++it, 	it++, =, *it, it-> (sağ taraf değeri) <br>it1 == it2		it1 != it2                                                                                                                                  | istream_iterator<br>istreambuf_iterator<br>                                          |
| Forward Iterator       | Copy constructible - Default Constructible<br>++it, 	it++, =, *it, it-> (sağ taraf değeri) ( sol taraf değeri)                                                                                                                 | forward_list<br>unordered_set unordered_multiset<br>unordered_map unordered_multimap |
| Bidirectional Iterator | Copy constructible - Default Constructible<br>++it, 	it++, --it, it--, = <br> *it, it-> (sağ taraf değeri) ( sol taraf değeri)                                                                                                | list<br>set multi_set<br>map multimap                                                |
| Random Access Iterator | ++it,	it++,		--it,	it--,	=<br>*it,    it->  (sağ taraf değeri) (sol taraf değeri)<br>it1 == it2		it1 != it2<br>it + n,		n + it,		it - n<br>it+=n,	it-=n<br>it1 - it2<br>it[n]<br>it1 < it2,   it1 <= it2,   it1 > it2,   it1 >= it2   | vector<br>deque<br>array<br>string<br>C array                                        |


```c++
//Examples
int main()
{
	vector<int>ivec{1,2,3,4,5}; // Random access iterator
	auto iter1 = ivec.begin();
	auto iter2 = ivec.end();
	++iter1;
	--iter1;
	--iter1[3];
	iter2 - iter1;
	iter + 2;

//-----------------------------------------------------------------------------------
	
    list<int>ilist{1,2,3,4,5}; //Bidirectional
								
	auto iter1 = ilist.begin();
	iter +1 ; //hata
	iter -2 ; //hata
	iter1.end()  - iter2.begin() ; //hata
	auto iter = ilist.end();
	--iter; //  GEÇERLI.

//-----------------------------------------------------------------------------------

	forward_list<int>ilist{1,2,3,4,5}; // forward iterator
								
	auto iter1 = ilist.begin();
	iter +1 ; //hata
	iter -2 ; //hata
	iter1.end()  - iter2.begin() ; //hata
	auto iter = ilist.end();
	--iter; //  SENTAKS HATASI

//-----------------------------------------------------------------------------------

	list<int>ilist{1,2,3,4,5}; // 
	
	auto iter = ilist.begin();
	iter = ilist.end();   // assingment örneği verdi.
							// Tabloda yok ama yapıalbiliyor.Hoca ekleyecek

}
```

Const Iteratörler
-----------------------
```c++
int main()
{
	vector<int> ivec{2,4,5,6,7,8};
	const vector<int>::iterator iter = iter.begin();  //const bir sınıf nesnesi değeri değiştirilemez.Low level const a karşılık gelmiyor.
	// BURADA ITERATOR CONST OLARAK NİTELENMİŞ. YANİ İTERATOR DEĞİŞEMEZ. ++ITER SENTAKS HATASI OLUR 
	
	++iter; // Hata
	*iter = 45; // Geçerli
}
```

Nesneye salt okuma amaçli erişim istersek ```++iter olmalı veya iter +2 olmalı, *iter yine olmalı
*iter = 54; Bu sentaks hatası olmalı```.
Low level const semantiği const keyword ile değil bir adaptör sınıfla elde edilmiş.Böyle sınıfların ismi ```const iterator``` sınıfı.
Bu sınıflarda containerların nested type'ıdır.

```c++
vectore<int>::const_iterator iter = ivec.begin();
cout << *iter << "\n";
*iter = 23; // Hata. Sadece okuma amaçlı kullanılacak.
```

Eski C++ ta sınıfların ```begin()``` ve ```end()``` functionları vardı. Modern C++ auto type deduction gelmesiyle containerlara ```cbegin``` ve ```cend``` functionlarıda eklendi.
```
begin() ->  vectorun<int>::iterator
cbegin() -> vectorun<int>::const_iterator
```

```c++
int main()
{
	using namespace std;
	vector<int> ivec{2,34,5,6,7,8,9};
	for(auto iter = ivec.cbegin(); iter != ivec.cend(); ++iter)
	{
		*iter // yani sadece okuma amaçlı erişim var.
	}
 
}
 ```

Global ```std:begin(); std::end(); std::begin(); std::cend();``` karşiliklarida var.
C dizilerinde member fonskiyon olmadığından global fonskiyonlar kullanılıyor.

```c++
auto iterbeg = cbegin(ivec);
auto iterend = cend(ivec);
int main()
{
    using namespace std;
	vector<int> a{2,34,5,6,7,8,9};
	for(auto iter = cbegin(a); iter != cend(a); ++iter)
	{
		std::cout << *iter << "\n";
	}
}
 ```
 
İteratörleri eleman olarak alip onların interface'ini değiştiren (eleman olarak alarak yada kalitimla yapabilir.) siniflara iterator adaptörü deniyor. Dolayısıyla const iteratörlere aslinda bir iterator adaptörü diyebiliriz.

Reverse İterator Adapter
-----------------------
Reverse Iterator bir Adapter yani reverse_iterator iterator'ün adaptörü.
Sınıfların iterator türü dışında birde reverse iterator isimli nested type'ı var. Mesela vector sınıfının reverse_iterator sınıfı türünden bir nesne oluşturabiliriz.

```c++
 vector<int>::reverse_iterator iter = ivec.rbegin();	bu şekilde oluşturulur
```

 NOT : Reverse iteratör türüne sahip olması için en az bidirectional iterator ü desteklemesi gerekiyor.
 
 Sınıfın rbegin ve rend fonksiyonları reverse_iterator döndürüyor.
 ```c++
 auto iter = ivec.rbegin(); // auto yazılabilir.
 auto iter = ivec.rend(); // auto yazılabilir.
 ```

 ```rbegin()```  fonksiyonun return değeri olan iterator, aslında vectordeki son öğeden bir sonraki konumu tutuyor.

 ```++iter ``` yapınca aslında tam tersi  ```--iter ``` işlemi yapıyor. Normal iteratörün tam tersi gibi davranıyor. * ile bir önceki nesneye eriştiriyor.

```c++
 vector<int> ivec{10,20,30,40,50};
 
 for(auto iter = ivec.rbegin(); iter != iter.rend(); ++iter)
 {
	std::cout << *iter << " "; // 50 40 30 20 10
 }
 ```

reverse_iterator sınıfı bir adaptör sınıf yani bu sınıf gerçek bir iteratör kullaniyor içinde. Bu da gerçek konumu tutuyor.
reverse_iterator, iter konumunu tuttuğunda fiziksel olarak, * operatörü ondan bir önceki nesnenin konumuna eriştiriyor. Yani  ```rbegin()``` gerçek olarak containerdaki  ```end``` konumunu tutuyor ama * operatörü o konumdaki nesneye değil o konumdan bir önceki nesneye eriştiriyor.

Neden böyle bir hile var?
 - Bu olmasaydi dizilerle ilgili kullanimda şöyle bir sorun olurdu, dizinin bittiği yerin adresini kullanmak legal, dereference etmediğimiz sürece legal.
Ama dizinin ilk elemanindan önce olmayan elemanin adresini kullanmak diye bir şey yok. reverse_iterator ```rbegin()``` functionunu çağırdığımızda aslında kullandiğimiz iteratör fiziksel olarak son öğeden sonrasini tutan iterator.doğrudan dereference etmek tanımsız davranış(UB).
 
```rend()``` fonskiyonu ilk öğenin konumunu tutuyor, dereference yapıldığında, olmayan yani dizinin başladiği yerden bir önceki nesneye gidip dereference edecek(UB).
Sınıfın implementasyonunda reverse_iterator sınıfı, normal iteratör sinifindan kalıtımla elde ediliyor. Sınıfın ```base()``` isimli bir fonskiyonu vardır, bu fonskiyon reverse_iterator'ün
sarmaladiği gerçek iteratörü döndürüyor. Yani dereference edince bir tane geri gelip dereference etmiyor. Tuttuğu yeri döndürüyor. 

```c++
int main()
{
	vector<int> ivec{10,20,30,40,50};
	auto riter = ivec.rbegin(); // riter, end konumunu tutuyor
	std::cout << *riter<< "\n"; // bir önceki nesneye erişim sağladığı için UB yok. end i tutuyordu. end ten bir öncekine gelip dereference edilir. 50 yazar
	auto iter = riter.base(); //  reverse iteratör yerine normal iteratör alacağım.end konumunu tutuyor 
	std::cout << *iter<< "\n"; // burada end konumunda dereference eder ve UNDEFINED BEHAVIOR
 
	std::cout << boolalpha << (iter = ivec.end()); // true yazdırır.

//-------------------------------------

    vector<int> ivec{10,20,30,40,50};
	auto riter = ivec.rbegin(); // end i tutuyor
	++riter;	// dizinin son öğesinde şuanda
	std::cout << *riter<< "\n"; // bir önceki öğeye gelip dereference eder.Yani 40
	auto iter = riter.base(); // Halen gerçek konumu 50 nin konumu
	std::cout << *iter<< "\n"; // bu konumu dereference ederse 50 yazar.Reverse iteratordeki gibi bir önceye gelip dereference etmiyor.
}
```

Böyle bir tasarımın nedeni şudur. Eğer bir reverse_iterator range'ini bir algoritmaya argüman olarak gönderirsek, algoritma o range'i sondan başa doğru işler. Ayni range in base'ini ayni algoritmaya gönderirsek baştan sona o range i dolaşmiş oluruz.
```c++
int main()
{
	vector<int> ivec{10,20,30,40,50};
	auto rbeg = ivec.rbegin();
	auto rend = ivec.rend();
	print(rbeg, rend); // print sondan başa doğru yazdırır.
	print(rend.base(), rbeg.base()); // bu sefer range in tersini oluşturmuş oluyorum.
}
```

```c++
// Question?
int main()
{
	vector<int> ivec{2,4,5,7,8,4,5,6,2,4};
	int ival = 2; // 2 yi containerdan sil
	auto iter = find(ivec.rbegin(),ivec.rend(),ival); // buradaki return değerde reverse iterator
	if(iter != ivec.rend())
	{
		ivec.erase(iter); // BURASI SENTAKS HATASI. ERASE VECTÖRÜN ITERATOR TÜRÜNDEN, REVERSE İTERATÖR TÜRÜNDEN DEĞİL. GERÇEK İTERATÖR
		std::cout << *iter << "\n"; // 2 YAZDIRDIĞINI GÖRÜRÜZ.(REVERSE ITERATÖR KULLANIYORUZ ŞUANDA)
		ivec.erase(iter.base()); // ŞİMDİ GERÇEK İTERE DÖNDÜ AMA FİND EDERKEN BİR ÖNCEKİ OPERATÖRÜ BULMUŞUK, DEREFERENCE EDİNCE O BİR ÖNCEKİNİ EDİYORDU. YANİ GERÇEK KONUM
								 // BİR SONRAKİ İTERATÖR.YANİ 4 Ü SİLDİK.
	
		ivec.erase(iter.base() - 1); // ŞİMDİ OLDU.
		print(ivec);
		
	}	
}
```

```c++
int main()
 {
	vector<string> svec;
	rfill(svec,10,rname);
	print(svec);
	sort(svec.begin(), svec.end()); // yazılar küçükten büyüğe sıralandı
	print(svec);
//------------------------------------
	vector<string> svec;
	rfill(svec,10,rname);
	print(svec);
	sort(svec.rbegin(), svec.rend()); //Burada ise büyükten küçüğe yapıldı sıralama
	print(svec);
 }
```

| Fonksiyon            | Return                            |
|----------------------|-----------------------------------|
| begin()    - end()   | container::iterator               |
| cbegin()   - cend()  | container::const_iterator         |
| rbegin()   - rendn() | container::reverse_iterator       |
| rcbegin()  - rcend() | container::const_reverse_iterator |


Iteratorler üzeri̇nde i̇şlem yapan algori̇tmalar
---------------------------------------------

| Fonksiyonlar               |          
|----------------------------|
| ```std::advance()```       |
| ```std::distance()```      |
| ```std::next()```          |
| ```std::prev()```          |
| ```std::iter_swap()```     |

 
 Bunlar iterator header fileda tanımlanan fonskiyon şablonları
 Diyelimki bir iteratörü artırmak istiyorsam bunun için += operator functionu kullanabilirim.
 ```c++
 int main()
 {
	vector<int>svec{12,2,5,4,6,7,8,9,2,4}; 
	auto iter = ivec.begin();
	iter += 3; // Geçerli
	cout << *iter << "\n";
	list<int>ilist{12,2,5,4,6,7,8,9,2,4};
	auto iter = ivec.begin();
	iter += 3; // Hata += bidirectional iteratorler tarafından desteklenmiyor
	++iter;
	++iter;
	++iter; // başka bir şans yok
	
	advance(iter,3);
	advance(iter,n);  // Eğer bu iter eğer random access iterator ise beg+=3 çalışacak ama random access iterator değilse beg n kez artırılması koduna dönüşecek
 }
 ```
 Bu durumlarda compile timeda kullanılan metaprogramming hileleri ile aslında iteratörü 3 kere artırmak yerine ```advance(iter,3);``` kullanılacak. Advance compile timeda kod seçecek eğer iterator random access iterator ise burada çalışan kod iter += 3 kodu olacak ama iter random access iterator değilse bir döngü içinde iteratörü n kez artıracak. Bu sayede ++ veya += operandı yapmak yerine compile timedaki seçilen kodla en düşük maliyetle uygun kod çalışacak.

