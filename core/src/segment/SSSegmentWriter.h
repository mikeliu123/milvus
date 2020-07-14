// Licensed to the Apache Software Foundation (ASF) under one
// or more contributor license agreements.  See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  The ASF licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.

#pragma once

#include <src/db/meta/MetaTypes.h>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "db/SnapshotVisitor.h"
#include "segment/Types.h"
#include "storage/FSHandler.h"
#include "utils/Status.h"

namespace milvus {
namespace segment {

class SSSegmentWriter {
 public:
    explicit SSSegmentWriter(const engine::SegmentVisitorPtr& segment_visitor);

    Status
    AddVectors(const std::string& name, const std::vector<uint8_t>& data, const std::vector<doc_id_t>& uids);

    Status
    AddVectors(const std::string& name, const uint8_t* data, uint64_t size, const std::vector<doc_id_t>& uids);

    Status
    AddAttrs(const std::string& name, const std::unordered_map<std::string, uint64_t>& attr_nbytes,
             const std::unordered_map<std::string, std::vector<uint8_t>>& attr_data, const std::vector<doc_id_t>& uids);

    Status
    SetVectorIndex(const knowhere::VecIndexPtr& index);

    Status
    SetAttrsIndex(const std::unordered_map<std::string, knowhere::IndexPtr>& attr_index,
                  const std::unordered_map<std::string, int64_t>& attr_nbytes,
                  const std::unordered_map<std::string, engine::meta::hybrid::DataType>& attr_type);

    Status
    WriteBloomFilter(const std::string& file_path, const IdBloomFilterPtr& bloom_filter_ptr);

    Status
    WriteDeletedDocs(const std::string& file_path, const DeletedDocsPtr& deleted_docs);

    Status
    Serialize();

    Status
    Cache();

    Status
    GetSegment(SegmentPtr& segment_ptr);

    Status
    Merge(const std::string& segment_dir_to_merge, const std::string& name);

    size_t
    Size();

    size_t
    VectorCount();

    Status
    WriteVectorIndex(const std::string& location);

    Status
    WriteAttrsIndex();

    void
    SetSegmentName(const std::string& name);

 private:
    Status
    WriteUids(const std::string& file_path, const std::vector<doc_id_t>& uids);

    Status
    WriteVectors(const std::string& file_path, const std::vector<uint8_t>& raw_vectors);

    Status
    WriteAttrs();

    Status
    WriteBloomFilter(const std::string& file_path);

    Status
    WriteDeletedDocs(const std::string& file_path);

 private:
    engine::SegmentVisitorPtr segment_visitor_;
    storage::FSHandlerPtr fs_ptr_;
    SegmentPtr segment_ptr_;
};

using SSSegmentWriterPtr = std::shared_ptr<SSSegmentWriter>;

}  // namespace segment
}  // namespace milvus