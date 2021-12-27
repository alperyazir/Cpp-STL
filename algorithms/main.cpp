#include <vector>
#include <list>
#include <iostream>

//copy
template<typename InIter, typename OutIter>
OutIter Copy(InIter beg, InIter end, OutIter dest_beg)
{
    while (beg != end)
    {
        *dest_beg++ = *beg++;
    }
    return dest_beg;
}

template<typename InIter, typename OutIter, typename UnPred>
OutIter Copy_if(InIter beg, InIter end, OutIter dest_beg, UnPred fn)
{
    while (beg != end)
    {
        if (fn(*beg))
        {
            *dest_beg++  = *beg;
        }
        beg++;
    }
    return dest_beg;
}

//---------------------------------------------------------";

// find
template<typename InIter, typename T>
InIter Find(InIter beg, InIter end, const T &tval)
{
    while (beg != end)
    {
        if (*beg == tval)
        {
            return beg;
        }
        ++beg;
    }
    return beg;
}

template<typename InIter, typename Pred>
InIter Find_if(InIter beg, InIter end, Pred pred)
{
    while (beg != end)
    {
        if (pred(*beg))
        {
            return beg;
        }
        ++beg;
    }
    return beg;
}
//---------------------------------------------------------";

// count
template<typename InIter, typename T>
size_t Count(InIter beg, InIter end, T tval)
{
    size_t counter {};
    while (beg != end)
    {
        if (*beg++ == tval)
        {
          ++counter;
        }
    }
    return counter;
}

template<typename InIter, typename UnPred>
size_t Count_if(InIter beg, InIter end, UnPred fn)
{
    size_t counter {};
    while (beg != end)
    {
        if (fn(*beg++))
        {
          ++counter;
        }
    }
    return counter;
}
//---------------------------------------------------------";

// print containers
template<typename Cont>
void print(const Cont &cont, const std::string &front_message = "")
{
    if(!front_message.empty())
    {
      std::cout << front_message << "\n";
    }
    auto iter = cont.begin();
    while (iter != cont.end())
    {
        std::cout << *iter++ << " ";
    }
    std::cout << "\n------------------------------\n";
}

int main()
{
    std::vector<int> ivec{1, 2, 3, 4, 5, 6};
    print(ivec, "Original List: ");
    std::list<int> ilist_copy(ivec.size());

    // copy
    Copy(ivec.begin(), ivec.end(), ilist_copy.begin());
    print(ilist_copy, "Copy Algorithm: ");

    // copy_if
    std::list<int> ilist_copy_if(ivec.size());
    Copy_if(ivec.begin(), ivec.end(), ilist_copy_if.begin(),
            [](int elem) { return elem > 2; });
    print(ilist_copy_if, "Copy_if Algorithm: ");

    //count
    auto size = Count(ivec.begin(), ivec.end(), 3);
    std::cout << "Count Algorithm: \nSize: " << size << "\n";
    std::cout << "-----------------\n";

    //count_if
    size = Count_if(ivec.begin(), ivec.end(), [](auto elem) { return elem > 2; });
    std::cout << "Count_if Algorithm: \nSize: " << size << "\n";
    std::cout << "-----------------\n";

    //find
    if (auto iter = Find(ivec.begin(), ivec.end(), 2); iter != ivec.end())
    {
        std::cout << "Found: " << *iter << " at index :" << iter - ivec.begin() << "\n";
    }
    std::cout << "-----------------\n";

    //find_f
    if (auto iter = Find_if(ivec.begin(), ivec.end(), [](auto elem)
                            { return elem > 2; });
        iter != ivec.end())
    {
        std::cout << "Found: " << *iter << " at index :" << iter - ivec.begin() << "\n";
    }
    std::cout << "-----------------\n";
}
