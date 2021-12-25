#ifndef ELGAMALADAPTER_H
#define ELGAMALADAPTER_H

#include "core/elgamal.h"
#include "core/md5.h"
#include <istream>

struct AsyncResult {
    PublicKey<InfInt> pk;
    SignedMessage<InfInt> sm;
};

class ElGamalAdapter
{
private:
public:
    ElGamalAdapter();
    AsyncResult sign(std::istream *input, PrivateKey<InfInt> privkey);
    bool verify(std::istream *input, SignedMessage<InfInt> sm, PublicKey<InfInt> pubkey);
};

#endif // ELGAMALADAPTER_H
