import hashlib
import struct
import time

def sha256d(data):
    return hashlib.sha256(hashlib.sha256(data).digest()).digest()

def create_genesis_block(timestamp, psz_timestamp, nbits, nonce, version, reward):
    # Construct the genesis transaction
    psz_timestamp_hex = ''.join(format(ord(c), 'x') for c in psz_timestamp)
    tx_new = (
        '01000000'  # Transaction version
        '01'        # Number of inputs
        + '00' * 32   # Previous output (none)
        + 'ffffffff'  # Previous output index
        + '4d'        # Script length
        + '04ffff001d0104' + format(len(psz_timestamp), 'x') + psz_timestamp_hex  # Coinbase script
        + 'ffffffff'  # Sequence
        + '01'        # Number of outputs
        + format(reward, 'x').zfill(16)  # Output value (50 BTC)
        + '23'        # Script length
        + '4104678afdb0fe5548271967f1a67130b7105cd6a828e03909a67962e0ea1f61deb649f6bc3f4cef38c4f35504e51ec112de5c384df7ba0b8d578a4c702b6bf11d5fac'  # Output script
        + '00000000'  # Lock time
    )
    tx_hash = sha256d(bytes.fromhex(tx_new))

    # Construct the genesis block
    genesis_block = (
        struct.pack('<L', version) +
        bytes.fromhex('00' * 32) +  # hashPrevBlock
        tx_hash +                   # hashMerkleRoot
        struct.pack('<L', timestamp) +
        struct.pack('<L', nbits) +
        struct.pack('<L', nonce)
    )

    genesis_hash = sha256d(genesis_block)

    return genesis_hash[::-1].hex(), nonce, tx_hash[::-1].hex()

if __name__ == "__main__":
    psz_timestamp = "The Times 03/Jan/2018 Bitcoin is name of the game for new generation of firms"
    nbits = 0x1e0ffff0
    version = 1
    reward = 50 * 10**8  # 50 BTC

    # Find a valid nonce
    nonce = 0
    timestamp = int(time.time())
    while True:
        genesis_hash, nonce, merkle_root = create_genesis_block(timestamp, psz_timestamp, nbits, nonce, version, reward)
        if genesis_hash.startswith('0000'):  # Adjust this difficulty as needed
            print(f"Genesis Block Hash: {genesis_hash}")
            print(f"Nonce: {nonce}")
            print(f"Merkle Root: {merkle_root}")
            print(f"Timestamp: {timestamp}")
            break
        nonce += 1
