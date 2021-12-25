#include "elgamaladapter.h"

ElGamalAdapter::ElGamalAdapter()
{

}

AsyncResult ElGamalAdapter::sign(std::istream *input, PrivateKey<InfInt> privkey)
{
    uint32_t *hash = md5(input);
    ElGamal<InfInt> elgamal(privkey);

    InfInt intHash = hash[0];
    intHash *= InfInt(0x100000000);
    intHash += hash[1];
    intHash *= InfInt(0x100000000);
    intHash += hash[2];
    intHash *= InfInt(0x100000000);
    intHash += hash[3];

    SignedMessage<InfInt> sm = elgamal.sign(intHash);

    PublicKey<InfInt> publicKey = elgamal.genPublicKey();

    AsyncResult result;
    result.pk = publicKey;
    result.sm = sm;

    return result;
}

bool ElGamalAdapter::verify(std::istream *input, SignedMessage<InfInt> sm, PublicKey<InfInt> pubkey)
{
    uint32_t *hash = md5(input);

    InfInt intHash = hash[0];
    intHash *= InfInt(0x100000000);
    intHash += hash[1];
    intHash *= InfInt(0x100000000);
    intHash += hash[2];
    intHash *= InfInt(0x100000000);
    intHash += hash[3];

    sm.m = intHash;

    return ElGamal<InfInt>::verify(sm, pubkey);
}
