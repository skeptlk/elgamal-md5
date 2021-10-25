#ifndef ELGAMAL_H
#define ELGAMAL_H

#include "InfInt.h"
#include <random>

template<typename T>
T gcd(T n1, T n2);

InfInt power(InfInt base, InfInt n);

InfInt inverse(InfInt k, InfInt n);

struct PublicKey {
    InfInt p;       // large prime
    InfInt alpha;   // primitive root
    InfInt beta;    // = alpha ** z mod p
};

struct PrivateKey
{
    InfInt z;       // secret integer
    InfInt p;       // large prime
    InfInt alpha;   // primitive root
};

struct SignedMessage {
    InfInt m; // message hash
    InfInt r; // = alpha**k mod p
    InfInt s; // = inv[k] * (m - z * r)
};


class ElGamal {
private:
    InfInt p, alpha, beta, z;
    std::uniform_real_distribution<float> *distr;
    std::default_random_engine *eng;
public:
    ElGamal(PrivateKey key);
    SignedMessage sign(InfInt m);
    InfInt keygen();
    PublicKey genPublicKey();

    static bool verify(SignedMessage sign, PublicKey pk);
};


#endif // ELGAMAL_H
