#include <iostream>
#include <unordered_map>
#include <string>
#include <sstream>
#include <unistd.h>
#include <vector>
#include <malloc.h>

using namespace std;

struct Object
{
    vector<string> names;
    unordered_map<string, int> dict;

    Object()
    {
        names.push_back("one");
        names.push_back("one thousand");
        names.push_back("one million");
        for (auto x : names)
        {
            dict[x] = x.size();
        }
    }
};

size_t getCurrentRSS()
{

    /* Linux ---------------------------------------------------- */
    long rss = 0L;
    FILE *fp = NULL;
    if ((fp = fopen("/proc/self/statm", "r")) == NULL)
        return (size_t)0L; /* Can't open? */
    if (fscanf(fp, "%*s%ld", &rss) != 1)
    {
        fclose(fp);
        return (size_t)0L; /* Can't read? */
    }
    fclose(fp);
    return (size_t)rss * (size_t)sysconf(_SC_PAGESIZE);
}

// https://man7.org/linux/man-pages/man3/mallinfo.3.html
void display_mallinfo2(void)
{
    struct mallinfo2 mi;

    mi = mallinfo2();

    printf("Total non-mmapped bytes (arena):       %zu\n", mi.arena);
    printf("# of free chunks (ordblks):            %zu\n", mi.ordblks);
    printf("# of free fastbin blocks (smblks):     %zu\n", mi.smblks);
    printf("# of mapped regions (hblks):           %zu\n", mi.hblks);
    printf("Bytes in mapped regions (hblkhd):      %zu\n", mi.hblkhd);
    printf("Max. total allocated space (usmblks):  %zu\n", mi.usmblks);
    printf("Free bytes held in fastbins (fsmblks): %zu\n", mi.fsmblks);
    printf("Total allocated space (uordblks):      %zu\n", mi.uordblks);
    printf("Total free space (fordblks):           %zu\n", mi.fordblks);
    printf("Topmost releasable block (keepcost):   %zu\n", mi.keepcost);
}

void work()
{
    const int n = 100000;
    unordered_map<int, Object> data;
    for (int i = 0; i < n; i++)
    {
        data[i] = Object();
    }
}

int main(int argc, char const *argv[])
{
    const int n = 5;
    for (int i = 0; i < n; i++)
    {
        cout << "loop #" << i << endl;
        work();
        display_mallinfo2();
        cout << endl;
        malloc_stats();
        cout << "rss = " << getCurrentRSS() << endl;
        cin.get();
    }
    return 0;
}
