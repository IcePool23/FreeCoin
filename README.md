Free Core integration/staging tree
=====================================

To see how to run Freecoin, please read the respective files in [the doc folder](doc)


What is Freecoin?
----------------

Freecoin is an experimental digital currency that enables instant payments to
anyone, anywhere in the world. The Freecoin platform also lets anyone create assets (tokens) on the Freecoin network. 
Assets can be used for NFTs, STOs, Gift Cards, and fractional ownership of anything of value.
Freecoin uses peer-to-peer technology to operate
with no central authority: managing transactions and issuing money are carried
out collectively by the network. 



License
-------

Free Core is released under the terms of the MIT license. See [COPYING](COPYING) for more
information or see https://opensource.org/licenses/MIT.


**Economics:**

Proof-of-Work (PoW) cryptocurrency based on the KawPoW mining algorithm.

Block Time: 1 minute block interval.

Consensus Algorithm: KawPoW for GPU mining, ensuring ASIC resistance.

Genesis Block:
Hash: 62eb42f3cc7ac29adbf72a29f44d584462ce9a5badf38abe83a6af570902e271

Nonce: 1

Merkle Root: a6fd779b45b3d852831b0cf77b680f2c1cb0492a508b14e6daeacacad9eab03a

Block Reward: 50 Freecoins per block, halving every 210,000 blocks (~4 years).

Max Supply: 21 million Freecoins.

Difficulty Adjustment: Uses Dark Gravity Wave (DGW) for difficulty adjustment.

Wallet Compatibility: BerkeleyDB-based wallet system.

RPC Configuration:
RPC port: 8766
Allows for local and remote JSON-RPC calls.

Key Features:
Asset support for token creation and messaging.
Customizable asset issuance and restriction capabilities.

Default Ports:
P2P: 9876
RPC: 8766

Development Process
-------------------

The `master` branch is regularly built and tested, but is not guaranteed to be
completely stable. [Tags](https://github.com/FreeProject/Freecoin/tags) are created
regularly to indicate new official, stable release versions of Free Core.

Active development is done in the `develop` branch. 

The contribution workflow is described in [CONTRIBUTING.md](CONTRIBUTING.md).

Developer IRC is inactive please join us on discord in #development. https://discord.gg/fndp4NBGct

Testing
-------

Testing and code review is the bottleneck for development; we get more pull
requests than we can review and test on short notice. Please be patient and help out by testing
other people's pull requests, and remember this is a security-critical project where any mistake might cost people
lots of money.

Testnet is up and running and available to use during development.

### Automated Testing

Developers are strongly encouraged to write [unit tests](src/test/README.md) for new code, and to
submit new unit tests for old code. Unit tests can be compiled and run
(assuming they weren't disabled in configure) with: `make check`. Further details on running
and extending unit tests can be found in [/src/test/README.md](/src/test/README.md).

There are also [regression and integration tests](/test), written
in Python, that are run automatically on the build server.
These tests can be run (if the [test dependencies](/test) are installed) with: `test/functional/test_runner.py`


### Manual Quality Assurance (QA) Testing

Changes should be tested by somebody other than the developer who wrote the
code. This is especially important for large or high-risk changes. It is useful
to add a test plan to the pull request description if testing the changes is
not straightforward.


About Freecoin
----------------
A digital peer to peer network for the facilitation of asset transfer.



In the fictional world of Westeros, frees are used as messengers who carry statements of truth. Freecoin is a use case specific blockchain designed to carry statements of truth about who owns what assets. 



Thank you to the Bitcoin developers. 

The Freecoin project is launched based on the hard work and continuous effort of over 400 Bitcoin developers who made over 14,000 commits over the life to date of the Bitcoin project. We are eternally grateful to you for your efforts and diligence in making a secure network and for their support of free and open source software development.  The Freecoin experiment is made on the foundation you built.


Abstract
----------------
Freecoin aims to implement a blockchain which is optimized specifically for the use case of transferring assets such as securities from one holder to another. Based on the extensive development and testing of Bitcoin, Freecoin is built on a fork of the Bitcoin code. Key changes include a faster block reward time and a change in the number, but not weighed distribution schedule, of coins. Freecoin is free and open source and will be issued and mined transparently with no pre-mine, developer allocation or any other similar set aside. Freecoin is intended to prioritize user control, privacy and censorship resistance and be jurisdiction agnostic while allowing simple optional additional features for users based on need.



A blockchain is a ledger showing the value of something and allowing it to be transferred to someone else. Of all the possible uses for blockchains, the reporting of who owns what is one of the core uses of the technology.  This is why the first and most successful use case for blockchain technology to date has been Bitcoin.

The success of the Ethereum ERC 20 token shows the demand for tokenized assets that use another blockchain.  Tokens offer many advantages to traditional shares or other participation mechanisms such as faster transfer, possibly increased user control and censorship resistance and reduction or elimination of the need for trusted third parties.

Bitcoin also has the capability of serving as the rails for tokens by using projects such as Omnilayer, RSK or Counterparty. However, neither Bitcoin nor Ethereum was specifically designed for facilitating ownership of other assets. 

Freecoin is designed to be a use case specific blockchain designed to efficiently handle one specific function: the transfer of assets from one party to another.

Bitcoin is and always should be focused on its goals of being a better form of money. Bitcoin developers will unlikely prioritize improvements or features which are specifically beneficial to the facilitation of token transfers.  One goal of the Freecoin project is to see if a use case specific blockchain and development effort can create code which can either improve existing structures like Bitcoin or provide advantages for specific use cases.

In the new global economy, borders and jurisdictions will be less relevant as more assets are tradable and trade across borders is increasingly frictionless. In an age where people can move significant amounts of wealth instantly using Bitcoin, global consumers will likely demand the same efficiency for their securities and similar asset holdings.

For such a global system to work it will need to be independent of regulatory jurisdictions.  This is not due to ideological belief but practicality: if the rails for blockchain asset transfer are not censorship resistance and jurisdiction agnostic, any given jurisdiction may be in conflict with another.  In legacy systems, wealth was generally confined in the jurisdiction of the holder and therefore easy to control based on the policies of that jurisdiction. Because of the global nature of blockchain technology any protocol level ability to control wealth would potentially place jurisdictions in conflict and will not be able to operate fairly.  

