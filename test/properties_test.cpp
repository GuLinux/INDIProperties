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
  Property<Switch> &s = properties["switch_name"];
  ASSERT_EQ("device name", s.device());
}
TEST_F(Test_INDIProperties, Update) {
  properties.add_switch("switch_name", nullptr, {"device name", "switch name", "label", "group"}, ISR_1OFMANY, [](ISState *, char **, int){ return true; } )
	    .add("prop name", "prop label", ISS_ON)
	    .add("prop name 2", "prop label 2", ISS_OFF);
  ISState new_states[] = {ISS_OFF, ISS_ON};
  char * texts[] = {"prop name", "prop name 2"};
  ASSERT_TRUE(properties.update("device name", "switch name", new_states, texts, 2) );
}
