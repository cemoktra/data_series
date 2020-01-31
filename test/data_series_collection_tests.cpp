#include <gtest/gtest.h>
#include <data_collection.hpp>

class data_collection_test : public ::testing::Test {
public:
    data_collection_test()
    {
    };

    ~data_collection_test() = default;

protected:
};

TEST_F(data_collection_test, serialization)
{
    // setup a test collection with all possible types
    auto collection = ds::data_collection::create_collection();
    auto constant_datas = collection->create_static_series(ds::data_series_properties(10, 10.0), 1.5);
    auto increasing_datas = collection->create_static_series({1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0}, 10.0);
    auto functional_datas = collection->create_functional_data_series(ds::functional_data_series::square_function, ds::data_series_properties(10, 10.0), increasing_datas);
    auto operational_datas = collection->create_operational_data_series(ds::operational_data_series::div_operation, {functional_datas, increasing_datas});
    auto mutable_datas = collection->create_mutable_data_series(10.0);

    // add data to mutable series
    mutable_datas->push_back({1.0, 2.0, 3.0, 4.0, 5.0});

    // serialize collection
    auto collection_json = collection->to_json(ds::data_encoding::base64);

    // deserialize collection
    auto collection_ = ds::data_collection::from_json(collection_json);

    // access deserialized datas
    auto constant_datas_ = collection_->data_series_by_id(constant_datas->id());
    auto increasing_datas_ = collection_->data_series_by_id(increasing_datas->id());
    auto functional_datas_ = collection_->data_series_by_id(functional_datas->id());
    auto operational_datas_ = collection_->data_series_by_id(operational_datas->id());
    auto mutable_datas_ = std::dynamic_pointer_cast<ds::mutable_data_series>(collection_->data_series_by_id(mutable_datas->id()));

    // expectations
    EXPECT_EQ(collection->id(), collection_->id());
    EXPECT_EQ(*constant_datas, *constant_datas_);
    EXPECT_EQ(*increasing_datas, *increasing_datas_);
    EXPECT_EQ(*functional_datas, *functional_datas_);
    EXPECT_EQ(*operational_datas, *operational_datas_);
    EXPECT_EQ(*mutable_datas, *mutable_datas_);
    
    EXPECT_TRUE(std::equal(constant_datas->begin(), constant_datas->end(), constant_datas_->begin()));
    EXPECT_TRUE(std::equal(increasing_datas->begin(), increasing_datas->end(), increasing_datas_->begin()));
    EXPECT_TRUE(std::equal(functional_datas->begin(), functional_datas->end(), functional_datas_->begin()));
    EXPECT_TRUE(std::equal(operational_datas->begin(), operational_datas->end(), operational_datas_->begin()));
    EXPECT_TRUE(std::equal(mutable_datas->begin(), mutable_datas->end(), mutable_datas_->begin()));

    EXPECT_EQ(mutable_datas->properties().samples(), mutable_datas_->properties().samples());
    mutable_datas_->push_back({1.0, 2.0, 3.0, 4.0, 5.0});
    EXPECT_GT(mutable_datas_->properties().samples(), mutable_datas->properties().samples());
    EXPECT_EQ(10, mutable_datas_->properties().samples());
}
