#include "gtest/gtest.h"
#include "indi_properties_map.h"
#include "indi_functions_impl.h"
#include "switch.h"
#include "c++/containers_streams.h"
using namespace INDI::Properties;
using namespace std;
using namespace GuLinux;

class Test_INDIPropertiesMap : public ::testing::Test {
public:
    PropertiesMap<string> properties_map;
};

TEST_F(Test_INDIPropertiesMap, Creation) {
    properties_map["hello"].add_switch("switch_name", nullptr, {"device name", "switch name", "label", "group"}, ISR_1OFMANY, [](const vector<Switch::UpdateArgs> &) {
        return false;
    } )
    .add("prop name", "prop label", ISS_ON)
    .add("prop name 2", "prop label 2", ISS_OFF);
    Property<Switch> &s = properties_map["hello"].switch_p("switch_name");
    ASSERT_EQ("device name", s.identity().device);
}

TEST_F(Test_INDIPropertiesMap, ClearAll) {
    properties_map["hello"].add_number("number_name", nullptr, {"device name", "number name", "label", "group"}, [](double *, char **, int) {
        return true;
    } )
    .add("prop name", "prop label", 0, 5, 1, 0)
    .add("prop name 2", "prop label 2", 1, 6, 2, 3);
    properties_map["bla"].add_switch("switch_name", nullptr, {"device name", "switch name", "label", "group"}, ISR_1OFMANY, [](const vector<Switch::UpdateArgs> &) {
        return true;
    } )
    .add("prop name", "prop label", ISS_ON)
    .add("prop name 2", "prop label 2", ISS_OFF);
    ASSERT_EQ(1, properties_map["hello"].numbers().size());
    ASSERT_EQ(1, properties_map["bla"].switches().size());
    properties_map.clear();
    ASSERT_EQ(0, properties_map["hello"].numbers().size());
    ASSERT_EQ(0, properties_map["bla"].switches().size());
}



TEST_F(Test_INDIPropertiesMap, Update) {
    const char *names[] = {"prop name"};
    ISState state = ISS_OFF;
    string text = "hello";
    double num = 3;
    properties_map["hello"].add_number("number_name", nullptr, {"device name", "number name", "label", "group"}, [&](Number::vtype *v, char **, int) {
        num = v[0];
        return true;
    } )
    .add("prop name", "prop label 2", 1, 6, 2, num);
    properties_map["bla"].add_switch("switch_name", nullptr, {"device name", "switch name", "label", "group"}, ISR_1OFMANY,
    [&](const vector<Switch::UpdateArgs> &v) {
        state = get<0>(v[0]);
        return true;
    } )
    .add("prop name", "prop label", state);
    properties_map["foo"].add_text("text_name", nullptr, {"device name", "text name", "label", "group"}, [&](Text::vtype* v, char **, int) {
        text = v[0];
        return true;
    } )
    .add("prop name", "prop label 2", text.c_str());
    ASSERT_TRUE(properties_map.update("device name", "number name", new Number::vtype[1] {1}, const_cast<char**>(names), 1));
    ASSERT_EQ(1, num);
    ASSERT_TRUE(properties_map.update("device name", "switch name", new Switch::vtype[1] {ISS_ON}, const_cast<char**>(names), 1));
    ASSERT_EQ(ISS_ON, state);
    ASSERT_TRUE(properties_map.update("device name", "text name", new Text::vtype[1] {"world"}, const_cast<char**>(names), 1));
    ASSERT_EQ("world", text);

}
