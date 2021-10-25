#include "elgamal.h"


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

InfInt power(InfInt base, InfInt n)
{
    InfInt r = base;
    while (--n > 0) r *= base;
    return r;
}

InfInt inverse(InfInt k, InfInt n)
{
    InfInt i = 1;
    while (i < n) {
        if ((i * k) % n == 1)
            return i;
        i++;
    }
    throw "Cannot find inv";
}


ElGamal::ElGamal(PrivateKey key)
{
       p = key.p;            // big prime
       alpha = key.alpha;    // primitive root
       z = key.z;            // secret key
       beta = power(alpha, z) % p;

       std::random_device rd;
       eng = new std::default_random_engine(rd());
       distr = new std::uniform_real_distribution<float>(0.0, 1.0);
}

SignedMessage ElGamal::sign(InfInt m) {
    SignedMessage res;

    InfInt key = keygen(); // key is not stored anywhere!
    InfInt r = power(alpha, key) % p;

    InfInt s = ((InfInt) m - r * z) * inverse(key, p - 1);
    while (s < 0) s += p - 1;
    s %= p - 1;

    res.s = s;
    res.m = m;
    res.r = r;

    //std::cout << "s=" << s << " m=" << m << " r=" << r << "\n";

    return res;
}

InfInt ElGamal::keygen() {
    return 31;
    InfInt k;
    do {
        k = (p * int(10000 * (*distr)(*eng))) / int(10000 * (*distr)(*eng));
        k %= p;
    } while (k <= 0 || gcd(k, p - 1) != 1);
    return k;
}

bool ElGamal::verify(SignedMessage sign, PublicKey pk) {
    InfInt v1 = pk.beta.powmod(sign.r, pk.p) * sign.r.powmod(sign.s, pk.p);
    v1 %= pk.p;
    InfInt v2 = pk.alpha.powmod(sign.m, pk.p);

    return v1 == v2;
}

PublicKey ElGamal::genPublicKey() {
    PublicKey pk;
    pk.p = p;
    pk.alpha = alpha;
    pk.beta = power(alpha, z) % p;

    return pk;
}
