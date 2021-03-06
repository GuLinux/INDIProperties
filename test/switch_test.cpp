#include "gtest/gtest.h"
#include "indi_property.h"
#include "indi_functions_impl.h"
#include "switch.h"
#include "c++/containers_streams.h"
using namespace INDI::Properties;
using namespace std;
using namespace GuLinux;
TEST(INDISwitch, AddProperty) {
  Property<Switch> my_prop{nullptr, {}, ISR_1OFMANY, [](const vector<Switch::UpdateArgs> &){ return false; }};
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
  Switch::vtype new_states[]{ISS_OFF, ISS_ON, ISS_OFF, ISS_OFF, ISS_OFF};
  const char *names[]{"name0", "name1", "name2", "name3", "name4"};
  ASSERT_EQ(0, IUUpdateSwitch(&vp, new_states, const_cast<char**>(names), 5));
  ASSERT_EQ(switches.data(), vp.sp);
  ASSERT_EQ(ISS_ON, vp.sp[1].s);
  ASSERT_EQ(ISS_ON, switches[1].s);
  ASSERT_EQ(ISS_OFF, switches[3].s);
}


TEST(INDISwitch, UpdateProperty) {
  bool retval = true;
  vector<Switch::UpdateArgs> states;
  auto update_f = [&](const vector<Switch::UpdateArgs> &new_states){
    for(int i=0; i<new_states.size(); i++)
      states = new_states;
    return retval;
  };
  const char *names[] = {"prop name", "prop name2"};
  Switch::vtype values[] = {ISS_OFF, ISS_ON};
  Property<Switch> my_prop{nullptr, {"device", "name"}, ISR_1OFMANY, update_f};
  my_prop.add("prop name", "prop label", ISS_ON)
	 .add("prop name2", "prop label2", ISS_OFF);
  ASSERT_TRUE(my_prop.update("device", "name", values, const_cast<char**>(names), 2));
  ASSERT_EQ( (vector<Switch::UpdateArgs>{ make_tuple(ISS_OFF, "prop name"), make_tuple(ISS_ON, "prop name2") }), states);
  ASSERT_EQ( (vector<Switch::vtype>{ISS_OFF, ISS_ON}), make_stream(my_prop.properties()).transform<vector<Switch::vtype>>([](ISwitch s){ return s.s; }).get() );
  retval = false;
  values[0] = ISS_ON;
  values[1] = ISS_OFF;
  ASSERT_FALSE(my_prop.update("device", "name", values, const_cast<char**>(names), 2));
  ASSERT_EQ( (vector<Switch::UpdateArgs>{make_tuple(ISS_ON, "prop name"), make_tuple(ISS_OFF, "prop name2")}), states);
}

TEST(INDISwitch, RunOnFirst) {
  Property<Switch> my_prop{nullptr, {}, ISR_NOFMANY, [](const vector<Switch::UpdateArgs> &){ return false; }};
  my_prop.add("prop name", "prop label", ISS_OFF)
         .add("prop name2", "prop label2", ISS_OFF)
         .add("prop name3", "prop label3", ISS_ON)
         .add("prop name4", "prop label4", ISS_ON);
  Switch::Entry entry;
  my_prop->first_on_switch([&](const Switch::Entry &e){ entry = e; });
  ASSERT_EQ(2, entry.index);
  ASSERT_STREQ("prop name3", entry.iswitch.name);
}

TEST(INDISwitch, RunOnAll) {
  Property<Switch> my_prop{nullptr, {}, ISR_NOFMANY, [](const vector<Switch::UpdateArgs> &){ return false; }};
  my_prop.add("prop name", "prop label", ISS_OFF)
          .add("prop name2", "prop label2", ISS_OFF)
          .add("prop name3", "prop label3", ISS_ON)
          .add("prop name4", "prop label4", ISS_ON);
  vector<Switch::Entry> entries;
  my_prop->on_switches([&](const Switch::Entry &e){ entries.push_back( e ); });
  ASSERT_EQ(2, entries.size());
  ASSERT_EQ(2, entries[0].index);
  ASSERT_STREQ("prop name3", entries[0].iswitch.name);
  ASSERT_EQ(3, entries[1].index);
  ASSERT_STREQ("prop name4", entries[1].iswitch.name);
}

TEST(INDISwitch, First) {
  Property<Switch> my_prop{nullptr, {}, ISR_NOFMANY, [](const vector<Switch::UpdateArgs> &){ return false; }};
  my_prop.add("prop name", "prop label", ISS_OFF)
         .add("prop name2", "prop label2", ISS_OFF)
         .add("prop name3", "prop label3", ISS_ON)
         .add("prop name4", "prop label4", ISS_ON);
  Switch::Entry entry;
  auto first = my_prop.first();
  ASSERT_TRUE(first);
  ASSERT_EQ(ISS_OFF, (*first).s);
}

TEST(INDISwitch, First_With_Function) {
  Property<Switch> my_prop{nullptr, {}, ISR_NOFMANY, [](const vector<Switch::UpdateArgs> &){ return false; }};
  my_prop.add("prop name", "prop label", ISS_OFF)
         .add("prop name2", "prop label2", ISS_OFF)
         .add("prop name3", "prop label3", ISS_ON)
         .add("prop name4", "prop label4", ISS_ON);
  Switch::Entry entry;
  auto first = my_prop.first([](const ISwitch &i){ return i.s == ISS_ON; });
  ASSERT_TRUE(first);
  ASSERT_EQ(ISS_ON, (*first).s);
  ASSERT_STREQ("prop name3", (*first).name );
}


TEST(INDISwitch, Find_With_Name) {
  Property<Switch> my_prop{nullptr, {}, ISR_NOFMANY, [](const vector<Switch::UpdateArgs> &){ return false; }};
  my_prop.add("prop name", "prop label", ISS_OFF)
         .add("prop name2", "prop label2", ISS_OFF)
         .add("prop name3", "prop label3", ISS_ON)
         .add("prop name4", "prop label4", ISS_ON);
  Switch::Entry entry;
  auto first = my_prop.find("prop name3");
  ASSERT_TRUE(first);
  ASSERT_EQ(ISS_ON, (*first).s);
  ASSERT_STREQ("prop name3", (*first).name );
}