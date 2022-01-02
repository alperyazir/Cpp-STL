## iteratörler

Pointer like bir interface'e sahip dolayısı ile bunların sınıf türünden nesneler olması durumunda yine pointer gibi 
kullanılmasını sağlayan operator overloading mekanizmaları vardır. Herbir iteratörün kategorisi olmak zorunda. Bu kategorilere göre iteratörlerin overload edilen mekanizmaları değişmektedir.

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
Buradaki nested type STL deki tanımlanmış empty classlardan biri.
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

### std::advance()
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

**Tag Dispatch** tekniğine uygun.

 ```c++
#include <iterator>
namespace details
{
	template <typename Raniter, typename Distance>
	void advance(Raniter& it , Distance n, std::random_access_iterator_tag)		
	{
		it += n;	
	}
	template <typename Biditer, typename Distance>
	void advance(Biditer& it , Distance n, std::bidirectional_iterator_tag)
	{
		if(n>0)
		{
			while(n--)
			{
				++it;
			}
		}
		else
		{
			while(n++)
			{
				--it;
			}
		}
	}
	template<typename Initer, typename Distance>
	void advance(Initer &it, Distance n, std::input_iterator_tag)
	{
		while(n--){
			++it;
		}
	}
}
template<typename Iter, typename Distance>
void advance(Iter& it, Distance n)
{
	details::advance(it, n, typename Iter::iterator_category{});
    /* 
        compile timeda vector<int> iteratörü olduğunu anlaşılırsa mesela, iterator türü random access iterator tag olcak ve 1. ci overload seçilecek
        Ama iter türü list<int> iteratör türü ise, iterator categori bidirectional olduğu için 2. overload seçilecek. Input ise 3.
        normalde ...::iterator::iterator_category derdik ama zaten buraya gelen bir iterator oldupu için yani ...::iterator türden gönderildiği için
        birdaha iterator yazmaya gerek yok.
    */
}

int main()
{
	int a[10]{};
	int *ptr = a;
	advance(ptr,5); //SENTAKS HATASI. Neden bu int * türünden ve bunun iterator categorisi yok.
    iterator_traits<vector<int>::iterator>::Iterator_category
	advance(ptr,5); //SENTAKS HATASI. Neden bu int * türünden ve bunun iterator categorisi yok.
}
```
iterator_traits pointer türleri için özelleştirilmiş.Dolayısıyla aslında iteratörün iterator categorisi demekle, iterator_traits in iterator açılımının iterator categorisi demek
aynı şey. Neden Böyle birşey yapmışlar ? Iterator traits pointer türleri için özelleştirilmiş.

```c++
template <typename Iter, typename Distance>
void advance(Iter& it , Distance n)
{
	advance(it, n, typename std::iterator_traits<Iter>::iterator_category{}); // Gerçek iterator türleri için aynı kategori ama pointer türleri için doğrudan pointer kullanılacak
	// bu da özelliştirmede random access iterator tag verecek.
}
```
Pointer hatasida böyle çözülmüş oluyor.
iterator_traits iterator header file içerisinde bulunuyor.

Aynı kodu compile time if kullanarak hiç tag dispatch uygulamadan gerçekleştirebiliriz?
 ```c++
#include <iterator>
#include <type_traits>
template <typename Iter, typename Dist>
void advance(Iter& pos, Dist n)
{
	using cat = typename std::iterator_traits<Iter>::iterator_category; // bunu birden fazla yazacağımız için türeş ismi verildi.
	if constexpr(std::is_same_v<cat,std::random_access_iterator_tag>)
	{
		pos += n;
	}
	else if constexpr(std::is_same_v<cat,std::bidirectional_iterator_tag>) is_same_v yerine is_same olsaydı en sona ::value gelecekti.
	{
		if(n>0)
		{
			while(n--)
			{
				++it;
			}
		}
		else
		{
			while(n++)
			{
				--it;
			}
		}
	}
	else // input iterator tag
	{
		while(n--)
			++pos;
	}
}
```

### std::distance()
İki iterator veya pointer arasındaki farkı bulmak için kullanılır.
Bunu compile timeda yaptırmak istersek arka planda tag dispatch veya if constexpr ile yapılabilir

```c++
#include <iterator>
int main()
{
	vector<int>vec{1,2,3,4,5,6,7};
	auto iter_x = vec.begin();
	auto iter_y = vec.end();
	
	auto n = distance(iter_x, iter_y);
	std::cout << "n = "<< n << "\n"; // 7

    int a[]{1,2,3,4,5,6,7,8,9};
	int* pf{a + 3}, *pe{a + 7};
	std::cout << distance(df,pe)<< "\n"; // 4 

    // distance mecburi senaryo

	forward_list<int> mylist {1,2,3,4,5,6,7,8,9}; // ileride görülecek bir nedenden size func ı yok. Diğer tüm containerlarda var ama bunda yok.Verimli olması için eklenmemiş.
	std::cout << distance(mylist.begin(),mylist.end()) << "\n"; // mylist in boyutunu veriyor.
}
```
### std::next() and std::prev
Advance pointerın veya iteratörün kendisini iletiyor yani referans yoluyla alıyor (call by reference). Fakat next bizden bir iterator alıyor ve o iteratörden n sonraki konumu return değeri olarak iletiyor. Call by value
```next(iter, 5); //call by value```. 
Bu iteratorden 5 sonraki konum. 2. parametresine argüman geçmezsek default olarak 1 alıyor.
 ```next(iter); // bir sonraki konumu return ediyor```
Prev de bunun tam tersi.
```prev(iter ,3);```
Bidirectional olmalı minimum. Bu konumların geçerli olması programcının sorumluluğunda. Exception throw etmiyorlar bu funclar.
```prev(iter);```
 birönceki konumu veriyor.

```c++
int main()
{
	vector <int> x {1,2,3,4,5,6,7};
	auto iter{x.begin()};
	auto iter_a = iter + 3; // 3 sonraki konum bu.Random access iterator bu
	//vector değilde list olsaydı sentaks hatası olurdu ama çözebiliyoruyz tabi
	auto iter_a = next(iter,3); //diyoruz. ITER DEĞIŞMIYOR HALEN AYNI DEĞERDE. ITER_A YA 3 SONRASI ATANDI !!!!!!
	std::cout << *iter <<"\n";
	//ilk ve son hariç hepsini yazdır
	print(next(x.begin()), prev(x.end()));
	//RANGE BASE FOR LOOPTA BİR BUG VAR.
	*prev(e.end(),2) = -1;  // 1,2,3,4,5,-1,7}
	print(x);
}
```

### std::iter_swap()
Iter swap doesnt swap iterators. İterator konumundaki 2 nesneyi swap ediyor.
```c++
template <typename Iter1, typename Iter2>
void iter_swap(Iter1 it_x , Iter2 it_y)
{
	....
}
```
```c++
// İlk ile son elemanı değiştirelim mesela
int main()
{
	list<int> x {1,2,3,4,5,6,7};
	swap(x.front(), x.back()); //aslında böyle yapılabilir. Front ve back, ilk ve son öğeye referans alabiliyoruz.
	swap(*x.begin(), *prev(e.end())); // end ten bir geri geldi. onu kullandi tabiki.
	iter_swap(x.begin(), prev(x.end())); //DİKKAT!!! Asıl örnek bu.konumları değil, konumdaki nesneleri takas ediyor.
	print(x);
}
```

```c++
// 2 değişkeni nasıl swap ederiz?
int x = 10;
int y = 45;
swap(x,y); // referans semantiği ile
iter_swap(&x,&y); // pointerlar iterator gibi kullanılabiliyorlar.
```

tuple sınıf şablonuyla call by value ilede swap edilebilir. 

// TODO
- ostream_iterator
- istream_iterator
- move_iterator