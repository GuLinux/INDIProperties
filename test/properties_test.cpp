#include "gtest/gtest.h"
#include "indi_properties.h"
#include "indi_functions_impl.h"
#include "switch.h"
#include "c++/containers_streams.h"
using namespace INDI::Properties;
using namespace std;
using namespace GuLinux;

class Test_INDIProperties : public ::testing::Test {
public:
  Properties<string> properties;
};

TEST_F(Test_INDIProperties, AddSwitch) {
  properties.add_switch("switch_name", nullptr, {"device name", "switch name", "label", "group"}, ISR_1OFMANY, [](ISState *, char **, int){ return false; } )
	    .add("prop name", "prop label", ISS_ON)
	    .add("prop name 2", "prop label 2", ISS_OFF);
  Property<Switch> &s = properties.switch_p("switch_name");
  ASSERT_EQ("device name", s.device());
}
TEST_F(Test_INDIProperties, UpdateSwitch) {
  properties.add_switch("switch_name", nullptr, {"device name", "switch name", "label", "group"}, ISR_1OFMANY, [](ISState *, char **, int){ return true; } )
	    .add("prop name", "prop label", ISS_ON)
	    .add("prop name 2", "prop label 2", ISS_OFF);
  ISState new_states[] = {ISS_OFF, ISS_ON};
  char * texts[] = {"prop name", "prop name 2"};
  ASSERT_TRUE(properties.update("device name", "switch name", new_states, texts, 2) );
}

TEST_F(Test_INDIProperties, AddNumber) {
  properties.add_number("number_name", nullptr, {"device name", "number name", "label", "group"}, [](double *, char **, int){ return false; } )
	    .add("prop name", "prop label", 0, 5, 1, 0)
	    .add("prop name 2", "prop label 2", 1, 6, 2, 3);
  Property<Number> &s = properties.number("number_name");
  ASSERT_EQ("device name", s.device());
}
TEST_F(Test_INDIProperties, UpdateNumber) {
  properties.add_number("number_name", nullptr, {"device name", "number name", "label", "group"}, [](double *, char **, int){ return true; } )
	    .add("prop name", "prop label", 0, 5, 1, 0)
	    .add("prop name 2", "prop label 2", 1, 6, 2, 3);
  double new_values[] = {2, 5};
  char * texts[] = {"prop name", "prop name 2"};
  ASSERT_TRUE(properties.update("device name", "number name", new_values, texts, 2) );
}

TEST_F(Test_INDIProperties, AddText) {
  properties.add_text("text_name", nullptr, {"device name", "number name", "label", "group"}, [](Text::vtype*, char **, int){ return false; } )
	    .add("prop name", "prop label", "hello")
	    .add("prop name 2", "prop label 2", "world");
  Property<Text> &s = properties.text("text_name");
  ASSERT_EQ("device name", s.device());
}
TEST_F(Test_INDIProperties, UpdateText) {
  properties.add_text("text_name", nullptr, {"device name", "number name", "label", "group"}, [](Text::vtype*, char **, int){ return true; } )
	    .add("prop name", "prop label", "hello")
	    .add("prop name 2", "prop label 2", "world");
  Text::vtype new_values[] = {"hello2", "world2"};
  char * texts[] = {"prop name", "prop name 2"};
  ASSERT_TRUE(properties.update("device name", "number name", new_values, texts, 2) );
}

TEST_F(Test_INDIProperties, ClearAll) {
    properties.add_number("number_name", nullptr, {"device name", "number name", "label", "group"}, [](double *, char **, int){ return true; } )
	    .add("prop name", "prop label", 0, 5, 1, 0)
	    .add("prop name 2", "prop label 2", 1, 6, 2, 3);
  properties.add_switch("switch_name", nullptr, {"device name", "switch name", "label", "group"}, ISR_1OFMANY, [](ISState *, char **, int){ return true; } )
	    .add("prop name", "prop label", ISS_ON)
	    .add("prop name 2", "prop label 2", ISS_OFF);
    ASSERT_EQ(1, properties.numbers().size());
    ASSERT_EQ(1, properties.switches().size());
    properties.clear();
    ASSERT_EQ(0, properties.numbers().size());
    ASSERT_EQ(0, properties.switches().size());
}


TEST_F(Test_INDIProperties, Clear) {
    properties.add_number("number_name", nullptr, {"device name", "number name", "label", "group"}, [](double *, char **, int){ return true; } )
	    .add("prop name", "prop label", 0, 5, 1, 0)
	    .add("prop name 2", "prop label 2", 1, 6, 2, 3);
    properties.numbers().clear();
    ASSERT_EQ(0, properties.numbers().size());
}