#ifndef POOL_H_DEFINED
#define POOL_H_DEFINED

/// Pre-allocated set of objects.
///
/// This is NOT an actual pool.
/// For info, see:
/// http://gamedevwithoutacause.com/?p=1169
///
/// ## For developers
/// Objects to use on this pool _must_ have a constructor with no
/// arguments! That's because I can't know how many/what args I will
/// send to the pre-allocated objects beforehand.
///
template<class T>
class Pool
{
public:
    Pool(unsigned int maxAmmount);
    virtual ~Pool();

    T getNew();
    void update(uint32_t dt);

private:
    std::vector<T> freeItems;
    std::vector<T> usedItems;

    unsigned int maxAmmount;
    unsigned int currentAmmount;
};

#endif //POOL_H_DEFINED

