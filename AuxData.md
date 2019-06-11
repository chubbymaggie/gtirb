Standard AuxData Schemata
=========================

The \ref AUXDATA_GROUP class provides generic storage for
application-specific data.

We specify a small number of standard gtirb::AuxData schemata to
support interoperability. These are listed below, in two sets:

- [Sanctioned](#sanctioned-auxdata-tables) Recommended for GTIRB
  users. Individual schemata are unlikely to change in future,
  although the set of Sanctioned schemata may grow.

- [Provisional](#provisional-auxdata-tables) Under consideration for
  'sanctioned' status.

For example, if you want to store alignment requirements for blocks
and data objects, you can use an [alignment](#alignment) table.

```c++
using namespace gtirb;
Context C;
IR& ir = *IR::Create(C);

// Attach an empty alignment table to the internal representation
ir.addAuxData("alignment", std::map<UUID, uint64_t>{});
ir.addModule(Module::Create(C));
Module& module = ir.modules()[0];

//...

// Create a new block
CFG& cfg = module.getCFG();
Block* b1 = emplaceBlock(cfg, C, Addr(464), 6);

// Record that the block should be aligned to 8-byte boundaries.
ir.getAuxData("alignment")->get<std::map<UUID, uint64_t>>()[b1->getUUID()] = 8;
```


## Sanctioned AuxData Tables

The following are the sanctioned AuxData table schemata.


| Label                                     | Type                                                          |
|-------------------------------------------|---------------------------------------------------------------|
| [`"functionBlocks"`](#functionblocks)     | ```std::map<gtirb::UUID, <std::set<gtirb::UUID>>```           |
| [`"functionEntries"`](#functionentries)   | ```std::map<gtirb::UUID, std::set<gtirb::UUID>>```            |
| [`"types"`](#types)                       | ```std::map<gtirb::UUID,std::string>```                       |
| [`"alignment"`](#alignment)               | ```std::map<gtirb::UUID, uint64_t>```                         |
| [`"comments"`](#comments)                 | ```std::map<std::pair<gtirb::UUID, uint64_t>, std::string>``` |
| [`"symbolForwarding"`](#symbolforwarding) | ```std::map<gtirb::Symbol,gtirb::Symbol>```                   |
| [`"padding"`](#padding)                   | ```std::map<gtirb::Addr, uint64_t>```                         |


### functionBlocks

| <!-- --> | <!-- -->                                                           |
|----------|--------------------------------------------------------------------|
| Label    | ```"functionBlocks"```                                             |
| Type     | ```std::map<gtirb::UUID, <std::set<gtirb::UUID>>```                |
| Key      | Function UUID.                                                     |
| Value    | The set of UUIDs of all the blocks (gtirb::Block) in the function. |


### functionEntries

| <!-- --> | <!-- -->                                                                       |
|----------|--------------------------------------------------------------------------------|
| Label    | ```"functionEntries"```                                                        |
| Type     | ```std::map<gtirb::UUID, <std::set<gtirb::UUID>>```                            |
| Key      | Function UUID.                                                                 |
| Value    | The set of UUIDs of all the block (gtirb::Block) entry points for the function |


### types

| <!-- --> | <!-- -->                                                                          |
|----------|-----------------------------------------------------------------------------------|
| Label    | ```"types"```                                                                     |
| Type     | ```std::map<gtirb::UUID,std::string>```                                           |
| Key      | The gtirb::UUID of a gtirb::DataObject.                                           |
| Value    | The type of the data, expressed as a std::string containing a C++ type specifier. |


### alignment

| <!-- --> | <!-- -->                                                                 |
|----------|--------------------------------------------------------------------------|
| Label    | ```"alignment"```                                                        |
| Type     | ```std::map<gtirb::UUID, uint64_t>```                                    |
| Key      | The gtirb::UUID of a gtirb::Block, gtirb::DataObject, or gtirb::Section. |
| Value    | Alignment requirements for the block/data object/section.                |


### comments

| <!-- --> | <!-- -->                                                                                                                                                                                                                                                                           |
|----------|------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| Label    | ```"comments"```                                                                                                                                                                                                                                                                   |
| Type     | ```gtirb::std::map<std::pair<gtirb::UUID, uint64_t>, std::string>>```                                                                                                                                                                                                              |
| Key      | A pair `(uuid, offset)` where `uuid` is the gtirb::UUID of a GTIRB entry and `offset` is an offset within that entry (in bytes). In the most typical use case, `uuid` will be the gtirb::UUID of a gtirb::Block and `offset` will be the offset of some instruction in that block. |
| Value    | A comment string relevant to the specified offset in the specified GTIRB entry.                                                                                                                                                                                                    |


### symbolForwarding

| <!-- --> | <!-- -->                                    |
|----------|---------------------------------------------|
| Label    | ```"symbolForwarding"```                    |
| Type     | ```std::map<gtirb::Symbol,gtirb::Symbol>``` |
| Key      | The "from" gtirb::Symbol.                   |
| Value    | The "to" gtirb::Symbol.                     |


### padding

| <!-- --> | <!-- -->                                       |
|----------|------------------------------------------------|
| Label    | ```"padding"```                                |
| Type     | ```std::map<gtirb::Addr, uint64_t>```          |
| Key      | An address at which padding has been inserted. |
| Value    | The length of the padding, in bytes.           |


## Provisional AuxData Tables

There are currently no provisional table schemata.
