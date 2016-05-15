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
  properties_map["hello"].add_switch("switch_name", nullptr, {"device name", "switch name", "label", "group"}, ISR_1OFMANY, [](ISState *, char **, int){ return false; } )
	    .add("prop name", "prop label", ISS_ON)
	    .add("prop name 2", "prop label 2", ISS_OFF);
  Property<Switch> &s = properties_map["hello"].switch_p("switch_name");
  ASSERT_EQ("device name", s.device());
}

TEST_F(Test_INDIPropertiesMap, ClearAll) {
  properties_map["hello"].add_number("number_name", nullptr, {"device name", "number name", "label", "group"}, [](double *, char **, int){ return true; } )
	    .add("prop name", "prop label", 0, 5, 1, 0)
	    .add("prop name 2", "prop label 2", 1, 6, 2, 3);
  properties_map["bla"].add_switch("switch_name", nullptr, {"device name", "switch name", "label", "group"}, ISR_1OFMANY, [](ISState *, char **, int){ return true; } )
	    .add("prop name", "prop label", ISS_ON)
	    .add("prop name 2", "prop label 2", ISS_OFF);
    ASSERT_EQ(1, properties_map["hello"].numbers().size());
    ASSERT_EQ(1, properties_map["bla"].switches().size());
    properties_map.clear();
    ASSERT_EQ(0, properties_map["hello"].numbers().size());
    ASSERT_EQ(0, properties_map["bla"].switches().size());
}
