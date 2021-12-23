#ifndef ELGAMAL_H
#define ELGAMAL_H

#include "InfInt.h"
#include <random>

template<typename T>
T gcd(T n1, T n2);

template<typename T>
T power(T base, T n);

template<typename T>
T inverse(T k, T n);

template<typename BInt>
struct PublicKey {
    BInt p;       // large prime
    BInt alpha;   // primitive root
    BInt beta;    // = alpha ** z mod p
};

template<typename BInt>
struct PrivateKey
{
    BInt z;       // secret integer
    BInt p;       // large prime
    BInt alpha;   // primitive root
};

template<typename BInt>
struct SignedMessage {
    BInt m; // message hash
    BInt r; // = alpha**k mod p
    BInt s; // = inv[k] * (m - z * r)
};


template<typename BInt>
class ElGamal {
private:
    BInt p, alpha, beta, z;
    std::uniform_real_distribution<float> *distr;
    std::default_random_engine *eng;
public:
    ElGamal(PrivateKey<BInt> key);
    SignedMessage<BInt> sign(BInt m);
    BInt keygen();
    PublicKey<BInt> genPublicKey();

    static bool verify(SignedMessage<BInt> sign, PublicKey<BInt> pk);
};




template<typename T>
T gcd(T n1, T n2)
{
    while(n1 != n2)
    {
        if(n1 > n2)
            n1 -= n2;
        else
            n2 -= n1;
    }
    return n1;
}

template<typename BInt>
BInt power(BInt base, BInt n)
{
    BInt r = base;
    while (--n > 0) r *= base;
    return r;
}

template<typename BInt>
BInt inverse(BInt a, BInt m)
{
    BInt m0 = m;
    BInt y = 0, x = 1;

    if (m == 1)
        return 0;

    while (a > 1) {
        // q is quotient
        BInt q = a / m;
        BInt t = m;

        // m is remainder now, process same as
        // Euclid's algo
        m = a % m, a = t;
        t = y;

        // Update y and x
        y = x - q * y;
        x = t;
    }

    // Make x positive
    if (x < 0)
        x += m0;

    return x;
}


template<typename BInt>
ElGamal<BInt>::ElGamal(PrivateKey<BInt> key)
{
    p = key.p;            // big prime
    alpha = key.alpha;    // primitive root
    z = key.z;            // secret key
    beta = alpha.powmod(z, p);

    std::random_device rd;
    eng = new std::default_random_engine(rd());
    distr = new std::uniform_real_distribution<float>(0.0, 1.0);
}


template<typename BInt>
SignedMessage<BInt> ElGamal<BInt>::sign(BInt m) {
    BInt key = keygen(); // key is not stored anywhere!
    BInt r = alpha.powmod(key, p);
    BInt inv = inverse<BInt>(key, p - 1);
    BInt s = ((BInt) m - r * z) % (p - 1);
    if (s < 0) s += p - 1;
    s = (s * inv) % (p - 1);

    std::cout << "s=" << s << " m=" << m << " r=" << r << "\n";

    return SignedMessage<BInt> { m, r, s };
}

template<typename BInt>
BInt ElGamal<BInt>::keygen() {
    return 31;
    BInt k;
    do {
        k = (p * int(10000 * (*distr)(*eng))) / int(10000 * (*distr)(*eng));
        k %= p;
    } while (k <= 0 || gcd(k, p - 1) != 1);
    return k;
}

template<typename BInt>
bool ElGamal<BInt>::verify(SignedMessage<BInt> sign, PublicKey<BInt> pk) {
    BInt v1 = pk.beta.powmod(sign.r, pk.p) * sign.r.powmod(sign.s, pk.p);
    v1 %= pk.p;
    BInt v2 = pk.alpha.powmod(sign.m, pk.p);

    return v1 == v2;
}

template<typename BInt>
PublicKey<BInt> ElGamal<BInt>::genPublicKey() {
    PublicKey<BInt> pk;
    pk.p = p;
    pk.alpha = alpha;
    pk.beta = alpha.powmod(z, p);

    return pk;
}

#endif // ELGAMAL_H
