#include "gtest/gtest.h"
#include "indi_property.h"
#include "indi_functions_impl.h"
#include "switch.h"
using namespace INDI::Properties;
using namespace std;
TEST(INDISwitch, AddProperty) {
  Property<Switch> my_prop{nullptr, {}, ISR_1OFMANY, [](ISState*, char**, int){ return false; }};
  my_prop.add("prop name", "prop label", ISS_ON);
  my_prop.add("prop name2", "prop label2", ISS_OFF);
  ASSERT_EQ(2, my_prop.properties().size());
  ASSERT_STREQ("prop label", my_prop.properties()[0].label);
  ASSERT_STREQ("prop label2", my_prop.properties()[1].label);
  ASSERT_EQ(ISS_ON, my_prop.properties()[0].s);
  ASSERT_EQ(ISS_OFF, my_prop.properties()[1].s);
  ASSERT_EQ(2, my_prop.vector_property().nsp);
  ASSERT_EQ(ISR_1OFMANY, my_prop.vector_property().r);
}

TEST(INDISwitch, UpdateProperty) {
  bool retval = false;
  vector<ISState> states(2);
  auto update_f = [&](ISState* new_states, char**, int n){
    for(int i=0; i<n; i++)
      states[i] = new_states[i];
    return retval;
  };
  char *names[] = {"prop name", "prop name2"};
  ISState values[] = {ISS_OFF, ISS_ON};
  Property<Switch> my_prop{nullptr, {"device", "name"}, ISR_1OFMANY, update_f};
  my_prop.add("prop name", "prop label", ISS_ON);
  my_prop.add("prop name2", "prop label2", ISS_OFF);
  ASSERT_FALSE(my_prop.update("device", "name", values, names, 2));
  ASSERT_EQ( (vector<ISState>{ISS_OFF, ISS_ON}), states);
  retval = true;
  values[0] = ISS_ON;
  values[1] = ISS_OFF;
  ASSERT_TRUE(my_prop.update("device", "name", values, names, 2));
  ASSERT_EQ( (vector<ISState>{ISS_ON, ISS_OFF}), states);
}