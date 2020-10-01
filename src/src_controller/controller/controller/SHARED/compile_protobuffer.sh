#!/bin/bash
~/Desktop/AtomicChessOS_BUILDROOT/src/src_buildroot/buildroot/output/host/bin/protoc --cpp_out . ./PROTOCOL_MSG.proto
protoc --cpp_out . ./PROTOCOL_MSG.proto
