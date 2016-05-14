#include "gtest/gtest.h"
#include "indi_property.h"
#include "indi_functions_impl.h"
#include "switch.h"
#include "c++/containers_streams.h"
using namespace INDI::Properties;
using namespace std;
using namespace GuLinux;
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

TEST(INDISwitch, NativeBehavior) {
  vector<ISwitch> switches;
  for(int i=0; i < 5; i++) {
    ISwitch s;
    stringstream n;
    n << "name" << i;
    IUFillSwitch(&s, n.str().c_str(), n.str().c_str(), ISS_OFF);
    switches.push_back(s);
  }
  ISwitchVectorProperty vp;
  IUFillSwitchVector(&vp, switches.data(), switches.size(), "dev", "name", "label", "group", IP_RW, ISR_NOFMANY, 60, IPS_OK);
  ASSERT_EQ(switches.data(), vp.sp);
  vp.sp[3].s = ISS_ON;
  ASSERT_EQ(ISS_ON, switches[3].s);
  ISState new_states[]{ISS_OFF, ISS_ON, ISS_OFF, ISS_OFF, ISS_OFF};
  char *names[]{"name0", "name1", "name2", "name3", "name4"};
  ASSERT_EQ(0, IUUpdateSwitch(&vp, new_states, names, 5));
  ASSERT_EQ(switches.data(), vp.sp);
  ASSERT_EQ(ISS_ON, vp.sp[1].s);
  ASSERT_EQ(ISS_ON, switches[1].s);
  ASSERT_EQ(ISS_OFF, switches[3].s);
}


TEST(INDISwitch, UpdateProperty) {
  bool retval = true;
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
  ASSERT_TRUE(my_prop.update("device", "name", values, names, 2));
  ASSERT_EQ( (vector<ISState>{ISS_OFF, ISS_ON}), states);
  ASSERT_EQ( (vector<ISState>{ISS_OFF, ISS_ON}), make_stream(my_prop.properties()).transform<vector<ISState>>([](ISwitch s){ return s.s; }).get() );
  retval = false;
  values[0] = ISS_ON;
  values[1] = ISS_OFF;
  ASSERT_FALSE(my_prop.update("device", "name", values, names, 2));
  ASSERT_EQ( (vector<ISState>{ISS_ON, ISS_OFF}), states);
}

TEST(INDISwitch, RunOnFirst) {
  Property<Switch> my_prop{nullptr, {}, ISR_NOFMANY, [](ISState*, char**, int){ return false; }};
  my_prop.add("prop name", "prop label", ISS_OFF);
  my_prop.add("prop name2", "prop label2", ISS_OFF);
  my_prop.add("prop name3", "prop label3", ISS_ON);
  my_prop.add("prop name4", "prop label4", ISS_ON);
  Switch::Entry entry;
  my_prop->first_on_switch([&](const Switch::Entry &e){ entry = e; });
  ASSERT_EQ(2, entry.index);
  ASSERT_STREQ("prop name3", entry.iswitch.name);
}

TEST(INDISwitch, RunOnAll) {
  Property<Switch> my_prop{nullptr, {}, ISR_NOFMANY, [](ISState*, char**, int){ return false; }};
  my_prop.add("prop name", "prop label", ISS_OFF);
  my_prop.add("prop name2", "prop label2", ISS_OFF);
  my_prop.add("prop name3", "prop label3", ISS_ON);
  my_prop.add("prop name4", "prop label4", ISS_ON);
  vector<Switch::Entry> entries;
  my_prop->on_switches([&](const Switch::Entry &e){ entries.push_back( e ); });
  ASSERT_EQ(2, entries.size());
  ASSERT_EQ(2, entries[0].index);
  ASSERT_STREQ("prop name3", entries[0].iswitch.name);
  ASSERT_EQ(3, entries[1].index);
  ASSERT_STREQ("prop name4", entries[1].iswitch.name);
}