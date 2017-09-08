#ifndef DATAFRAME_TRACKER_HPP
#define DATAFRAME_TRACKER_HPP
#include "core_fwd.hpp"

#include "interface/attribute_description.hpp"

#include <boost/intrusive/unordered_set.hpp>

#include <memory>

BEGIN_NAMESPACE_CORE

class dataframe_tracker
{
public:
  static constexpr std::size_t initial_bucket_count = 1024;
  dataframe_tracker(attribute_description const &description,
                    std::vector<std::string> const &key_names,
                    std::vector<std::string> const &val_names);

  void feed_record(attribute_list const &input);

private:
  using hook_type = boost::intrusive::unordered_set_base_hook<
      boost::intrusive::store_hash<true>>;

  struct node;
  struct node_hash
  {
    std::size_t const m_key_size;
    std::size_t operator()(dataframe_tracker::node const &n) const;
  };
  struct node_equal
  {
    std::size_t const &m_key_size;
    std::size_t operator()(dataframe_tracker::node const &lhs,
                           dataframe_tracker::node const &rhs) const;
  };

  using set_type =
      boost::intrusive::unordered_set<hook_type,                              //
                                      boost::intrusive::hash<node_hash>,      //
                                      boost::intrusive::equal<node_equal>,    //
                                      boost::intrusive::compare_hash<true>,   //
                                      boost::intrusive::power_2_buckets<true> //
                                      >;
  using bucket_type = set_type::bucket_type;
  std::unique_ptr<bucket_type[]> m_buckets;
  set_type m_attribute_set;
  attribute_description m_attribute_description;
  std::vector<std::size_t> const m_to_outer_indices;
};
END_NAMESPACE_CORE

#endif // DATAFRAME_TRACKER_HPP
