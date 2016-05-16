#include "gtest/gtest.h"
#include "indi_property.h"
#include "indi_functions_impl.h"
#include "number.h"
#include "c++/containers_streams.h"
using namespace INDI::Properties;
using namespace std;
using namespace GuLinux;
TEST(INDINumber, AddProperty) {
    Property<Number> my_prop {nullptr, {}, [](const vector<Number::UpdateArgs> &) {
        return false;
    }
                             };
    my_prop.add("prop name", "prop label", 0, 5, 1, 3);
    my_prop.add("prop name2", "prop label2", 0, 6, 2, 2);
    ASSERT_EQ(2, my_prop.properties().size());
    ASSERT_STREQ("prop label", my_prop.properties()[0].label);
    ASSERT_STREQ("prop label2", my_prop.properties()[1].label);
    ASSERT_EQ(3, my_prop.properties()[0].value);
    ASSERT_EQ(2, my_prop.properties()[1].value);
    ASSERT_EQ(2, my_prop.vector_property().nnp);
}


TEST(INDINumber, UpdateProperty) {
    bool retval = true;
    vector<Number::UpdateArgs> values;
    auto update_f = [&](const vector<Number::UpdateArgs> &new_values) {
        values = new_values;
        return retval;
    };
    const char *names[] = {"prop name", "prop name2"};
    Number::vtype new_values[] = {2, 4};
    Property<Number> my_prop {nullptr, {"device", "name"}, update_f};
    my_prop.add("prop name", "prop label", 0, 5, 1, 3)
    .add("prop name2", "prop label2", 0, 6, 2, 2);

    ASSERT_TRUE(my_prop.update("device", "name", new_values, const_cast<char**>(names), 2));
    ASSERT_EQ( (vector<Number::UpdateArgs> {make_tuple(2, "prop name"), make_tuple(4, "prop name2")}), values);
    ASSERT_EQ( (vector<Number::UpdateArgs> {make_tuple(2, "prop name"), make_tuple(4, "prop name2")}),
      make_stream(my_prop.properties()).transform<vector<Number::UpdateArgs>>([](INumber v) { return make_tuple(v.value, string{v.name}); }).get() );
    
    retval = false;
    new_values[0] = 0;
    new_values[1] = 0;
    ASSERT_FALSE(my_prop.update("device", "name", new_values, const_cast<char**>(names), 2));
    ASSERT_EQ( (vector<Number::UpdateArgs>{make_tuple(0, "prop name"), make_tuple(0, "prop name2")}), values);
  }

