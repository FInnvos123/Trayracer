#ifndef SINGLETON_H
#define SINGLETON_H

class Singleton
{
    public:
        Singleton(Singleton const&) = delete;
        void operator=(Singleton const&) = delete;

    protected:
        Singleton() = default;
        virtual ~Singleton() = default;
};

#endif /* ifndef SINGLETON_H */
