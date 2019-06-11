//===- CFG.test.cpp ---------------------------------------------*- C++ -*-===//
//
//  Copyright (C) 2018 GrammaTech, Inc.
//
//  This code is licensed under the MIT license. See the LICENSE file in the
//  project root for license terms.
//
//  This project is sponsored by the Office of Naval Research, One Liberty
//  Center, 875 N. Randolph Street, Arlington, VA 22203 under contract #
//  N68335-17-C-0700.  The content of the information does not necessarily
//  reflect the position or policy of the Government and no official
//  endorsement should be inferred.
//
//===----------------------------------------------------------------------===//
#include <gtirb/Block.hpp>
#include <gtirb/CFG.hpp>
#include <gtirb/Context.hpp>
#include <gtirb/ProxyBlock.hpp>
#include <proto/CFG.pb.h>
#include <gtest/gtest.h>

using namespace gtirb;

TEST(Unit_CFG, compilationIteratorTypes) {
  static_assert(std::is_same_v<cfg_iterator::reference, CfgNode&>);
  static_assert(std::is_same_v<const_cfg_iterator::reference, const CfgNode&>);
  {
    cfg_iterator it;
    const_cfg_iterator cit(it);
    cit = it;
  }

  static_assert(std::is_same_v<block_iterator::reference, Block&>);
  static_assert(std::is_same_v<const_block_iterator::reference, const Block&>);
  {
    block_iterator it;
    const_block_iterator cit(it);
    cit = it;
  }
}

static Context Ctx;

TEST(Unit_CFG, addVertex) {
  CFG Cfg;
  auto* B = Block::Create(Ctx, Addr(1), 2);
  auto Descriptor1 = addVertex(B, Cfg);
  EXPECT_EQ(Cfg[Descriptor1], B);
  EXPECT_EQ(dyn_cast<Block>(Cfg[Descriptor1])->getAddress(), Addr(1));
  EXPECT_EQ(dyn_cast<Block>(Cfg[Descriptor1])->getSize(), 2);

  // adding the same block again doesn't change the graph
  auto Descriptor2 = addVertex(B, Cfg);
  EXPECT_EQ(Descriptor2, Descriptor1);
  auto Vertices = vertices(Cfg);
  EXPECT_EQ(std::distance(Vertices.first, Vertices.second), 1);

  auto* P = ProxyBlock::Create(Ctx);
  auto Descriptor3 = addVertex(P, Cfg);
  EXPECT_EQ(Cfg[Descriptor3], P);
  auto Descriptor4 = addVertex(P, Cfg);
  EXPECT_EQ(Descriptor4, Descriptor3);
  Vertices = vertices(Cfg);
  EXPECT_EQ(std::distance(Vertices.first, Vertices.second), 2);
}

TEST(Unit_CFG, getVertex) {
  CFG Cfg;
  auto* B = Block::Create(Ctx, Addr(1), 2);
  auto* P = ProxyBlock::Create(Ctx);
  auto DescriptorB = addVertex(B, Cfg);
  auto DescriptorP = addVertex(P, Cfg);
  EXPECT_EQ(getVertex(B, Cfg), DescriptorB);
  EXPECT_EQ(getVertex(P, Cfg), DescriptorP);
}

TEST(Unit_CFG, cfgIterator) {
  CFG Cfg;
  auto* B1 = Block::Create(Ctx, Addr(1), 2);
  auto* P1 = ProxyBlock::Create(Ctx);
  auto* B2 = Block::Create(Ctx, Addr(3), 2);
  auto* P2 = ProxyBlock::Create(Ctx);
  addVertex(B1, Cfg);
  addVertex(P1, Cfg);
  addVertex(B2, Cfg);
  addVertex(P2, Cfg);

  // Non-const graph produces a regular iterator
  boost::iterator_range<cfg_iterator> NodeRange = nodes(Cfg);
  EXPECT_EQ(std::distance(NodeRange.begin(), NodeRange.end()), 4);
  auto It = NodeRange.begin();
  EXPECT_EQ(&*It, B1);
  ++It;
  EXPECT_EQ(&*It, P1);
  ++It;
  EXPECT_EQ(&*It, B2);
  ++It;
  EXPECT_EQ(&*It, P2);
  ++It;
  EXPECT_EQ(It, NodeRange.end());

  // Const graph produces a const iterator
  const CFG& ConstCfg = Cfg;
  boost::iterator_range<const_cfg_iterator> ConstRange = nodes(ConstCfg);
  EXPECT_EQ(std::distance(ConstRange.begin(), ConstRange.end()), 4);
  auto Cit = ConstRange.begin();
  EXPECT_EQ(&*Cit, B1);
  ++Cit;
  EXPECT_EQ(&*Cit, P1);
  ++Cit;
  EXPECT_EQ(&*Cit, B2);
  ++Cit;
  EXPECT_EQ(&*Cit, P2);
  ++Cit;
  EXPECT_EQ(Cit, ConstRange.end());
}

TEST(Unit_CFG, blockIterator) {
  CFG Cfg;
  addVertex(Block::Create(Ctx, Addr(1), 2), Cfg);
  addVertex(Block::Create(Ctx, Addr(3), 2), Cfg);
  addVertex(Block::Create(Ctx, Addr(5), 2), Cfg);
  addVertex(ProxyBlock::Create(Ctx), Cfg);

  // Non-const graph produces a regular iterator
  boost::iterator_range<block_iterator> BlockRange = blocks(Cfg);
  EXPECT_EQ(std::distance(BlockRange.begin(), BlockRange.end()), 3);
  auto It = BlockRange.begin();
  EXPECT_EQ(It->getAddress(), Addr(1));
  ++It;
  EXPECT_EQ(It->getAddress(), Addr(3));
  ++It;
  EXPECT_EQ(It->getAddress(), Addr(5));
  ++It;
  EXPECT_EQ(It, BlockRange.end());

  // Const graph produces a const iterator
  const CFG& ConstCfg = Cfg;
  boost::iterator_range<const_block_iterator> ConstRange = blocks(ConstCfg);
  EXPECT_EQ(std::distance(ConstRange.begin(), ConstRange.end()), 3);
  auto Cit = ConstRange.begin();
  EXPECT_EQ(Cit->getAddress(), Addr(1));
  ++Cit;
  EXPECT_EQ(Cit->getAddress(), Addr(3));
  ++Cit;
  EXPECT_EQ(Cit->getAddress(), Addr(5));
  ++Cit;
  EXPECT_EQ(Cit, ConstRange.end());
}

TEST(Unit_CFG, edges) {
  CFG Cfg;
  auto B1 = Block::Create(Ctx, Addr(1), 2);
  auto B2 = Block::Create(Ctx, Addr(3), 4);
  auto P1 = ProxyBlock::Create(Ctx);
  addVertex(B1, Cfg);
  addVertex(B2, Cfg);
  addVertex(P1, Cfg);

  auto E1 = addEdge(B1, P1, Cfg);
  EXPECT_EQ(Cfg[source(*E1, Cfg)], B1);
  EXPECT_EQ(Cfg[target(*E1, Cfg)], P1);

  auto E2 = addEdge(B2, P1, Cfg);
  EXPECT_EQ(Cfg[source(*E2, Cfg)], B2);
  EXPECT_EQ(Cfg[target(*E2, Cfg)], P1);

  auto E3 = addEdge(P1, B1, Cfg);
  EXPECT_EQ(Cfg[source(*E3, Cfg)], P1);
  EXPECT_EQ(Cfg[target(*E3, Cfg)], B1);

  // Parallel edge
  auto E4 = addEdge(B1, P1, Cfg);
  EXPECT_EQ(Cfg[source(*E4, Cfg)], B1);
  EXPECT_EQ(Cfg[target(*E4, Cfg)], P1);
}

TEST(Unit_CFG, edgeLabels) {
  CFG Cfg;
  auto B1 = Block::Create(Ctx, Addr(1), 2);
  auto B2 = Block::Create(Ctx, Addr(3), 4);
  addVertex(B1, Cfg);
  addVertex(B2, Cfg);

  // Create an edge with no label
  auto E = addEdge(B2, B1, Cfg);
  EXPECT_FALSE(Cfg[*E]);

  auto Conds = {ConditionalEdge::OnFalse, ConditionalEdge::OnTrue};
  auto Dirs = {DirectEdge::IsDirect, DirectEdge::IsIndirect};
  auto Types = {EdgeType::Branch, EdgeType::Call,    EdgeType::Fallthrough,
                EdgeType::Return, EdgeType::Syscall, EdgeType::Sysret};

  // Create a number of parallel edges with different labels.
  std::vector<CFG::edge_descriptor> Descriptors;
  for (ConditionalEdge Cond : Conds) {
    for (DirectEdge Dir : Dirs) {
      for (EdgeType Type : Types) {
        E = addEdge(B1, B2, Cfg);
        Cfg[*E] = std::make_tuple(Cond, Dir, Type);
        Descriptors.push_back(*E);
      }
    }
  }

  // Check that the edges have the properties we assigned.
  auto It = Descriptors.begin();
  for (ConditionalEdge Cond : Conds) {
    for (DirectEdge Dir : Dirs) {
      for (EdgeType Type : Types) {
        EXPECT_TRUE(Cfg[*It]);
        EXPECT_EQ(std::get<ConditionalEdge>(*Cfg[*It]), Cond);
        EXPECT_EQ(std::get<DirectEdge>(*Cfg[*It]), Dir);
        EXPECT_EQ(std::get<EdgeType>(*Cfg[*It]), Type);
        ++It;
      }
    }
  }
}

TEST(Unit_CFG, protobufRoundTrip) {
  CFG Result;
  proto::CFG Message;

  auto B1 = Block::Create(Ctx, Addr(1), 2, 3);
  auto B2 = Block::Create(Ctx, Addr(4), 5, 6);
  auto P1 = ProxyBlock::Create(Ctx);
  {
    CFG Original;
    addVertex(B1, Original);
    addVertex(B2, Original);
    addVertex(P1, Original);

    auto E1 = addEdge(B1, P1, Original);
    auto E2 = addEdge(B2, P1, Original);
    addEdge(P1, B1, Original);
    Original[*E1] = std::make_tuple(ConditionalEdge::OnTrue,
                                    DirectEdge::IsDirect, EdgeType::Branch);
    Original[*E2] = std::make_tuple(ConditionalEdge::OnFalse,
                                    DirectEdge::IsIndirect, EdgeType::Call);

    Message = toProtobuf(Original);
  }
  fromProtobuf(Ctx, Result, Message);

  auto Range = nodes(Result);
  EXPECT_EQ(std::distance(Range.begin(), Range.end()), 3);
  auto It = Range.begin();
  EXPECT_EQ(It->getUUID(), B1->getUUID());
  EXPECT_EQ(dyn_cast<Block>(&*It)->getAddress(), Addr(1));
  EXPECT_EQ(dyn_cast<Block>(&*It)->getSize(), 2);
  EXPECT_EQ(dyn_cast<Block>(&*It)->getDecodeMode(), 3);
  ++It;
  EXPECT_EQ(It->getUUID(), B2->getUUID());
  EXPECT_EQ(dyn_cast<Block>(&*It)->getAddress(), Addr(4));
  EXPECT_EQ(dyn_cast<Block>(&*It)->getSize(), 5);
  EXPECT_EQ(dyn_cast<Block>(&*It)->getDecodeMode(), 6);
  ++It;
  EXPECT_EQ(It->getUUID(), P1->getUUID());

  // Check edges
  EXPECT_TRUE(
      edge(*getVertex(B1, Result), *getVertex(P1, Result), Result).second);
  EXPECT_TRUE(
      edge(*getVertex(B2, Result), *getVertex(P1, Result), Result).second);
  EXPECT_TRUE(
      edge(*getVertex(P1, Result), *getVertex(B1, Result), Result).second);

  // Check nonexistent edges
  EXPECT_FALSE(
      edge(*getVertex(B1, Result), *getVertex(B2, Result), Result).second);
  EXPECT_FALSE(
      edge(*getVertex(B2, Result), *getVertex(B1, Result), Result).second);
  EXPECT_FALSE(
      edge(*getVertex(P1, Result), *getVertex(B2, Result), Result).second);

  // Check labels
  auto E1 = edge(*getVertex(B1, Result), *getVertex(P1, Result), Result).first;
  EXPECT_EQ(std::get<ConditionalEdge>(*Result[E1]), ConditionalEdge::OnTrue);
  EXPECT_EQ(std::get<DirectEdge>(*Result[E1]), DirectEdge::IsDirect);
  EXPECT_EQ(std::get<EdgeType>(*Result[E1]), EdgeType::Branch);

  auto E2 = edge(*getVertex(B2, Result), *getVertex(P1, Result), Result).first;
  EXPECT_EQ(std::get<ConditionalEdge>(*Result[E2]), ConditionalEdge::OnFalse);
  EXPECT_EQ(std::get<DirectEdge>(*Result[E2]), DirectEdge::IsIndirect);
  EXPECT_EQ(std::get<EdgeType>(*Result[E2]), EdgeType::Call);

  auto E3 = edge(*getVertex(P1, Result), *getVertex(B1, Result), Result).first;
  EXPECT_FALSE(Result[E3]);
}
