#ifndef DATAFRAME_TRACKER_HPP
#define DATAFRAME_TRACKER_HPP
#include "core_fwd.hpp"

#include "interface/attribute_description.hpp"
#include "interface/attribute_list.hpp"
#include "interface/dataframe.hpp"

#include <boost/intrusive/unordered_set.hpp>

#include <memory>

BEGIN_NAMESPACE_CORE

class dataframe_tracker : public dataframe
{
protected:
  using hook_type = boost::intrusive::unordered_set_base_hook<
      boost::intrusive::store_hash<true>>;

public:
  struct node : public dataframe_tracker::hook_type
  {
    static std::vector<attribute>
    make_attributes(attribute_list const &other_attributes,
                    std::vector<std::size_t> const &mapping);

    node(attribute_list const &other_attributes,
         std::vector<std::size_t> const &mapping);
    std::vector<attribute> const &attributes() const;
    std::vector<attribute> &attributes();
    void update(attribute_list const &new_values,
                std::vector<std::size_t> const &mapping, std::size_t key_size);

  private:
    std::vector<attribute> m_attributes;
  };

  struct node_hash
  {
    node_hash(std::size_t key_size) : m_key_size(key_size) {}
    std::size_t operator()(dataframe_tracker::node const &n) const;
    std::size_t operator()(attribute const &attr) const;
    std::size_t const m_key_size;
  };
  struct node_equal
  {
    node_equal(std::size_t key_size) : m_key_size(key_size) {}
    std::size_t const m_key_size;
    bool operator()(node const &lhs, node const &rhs) const;
    bool operator()(node const &node, attribute const &attr) const;
    bool operator()(attribute const &attr, node const &node) const;
    bool operator()(attribute const &lhs, attribute const &rhs) const;
  };

protected:
  using set_type =
      boost::intrusive::unordered_set<node,                                   //
                                      boost::intrusive::hash<node_hash>,      //
                                      boost::intrusive::equal<node_equal>,    //
                                      boost::intrusive::compare_hash<true>,   //
                                      boost::intrusive::power_2_buckets<true> //
                                      >;

  using bucket_type = set_type::bucket_type;
  std::unique_ptr<bucket_type[]> m_buckets;
  std::size_t const m_key_size;
  set_type m_attribute_set;
  attribute_description m_attribute_description;
  std::vector<std::size_t> const m_to_outer_indices;

public:
  static const std::size_t hash_seed = 0;

  using iterator = set_type::iterator;
  using const_iterator = set_type::const_iterator;
  using insert_commit_data = set_type::insert_commit_data;

  static constexpr std::size_t initial_bucket_count = 1024;
  dataframe_tracker(attribute_description const &description,
                    std::vector<std::string> const &key_names,
                    std::vector<std::string> const &val_names);
  ~dataframe_tracker();

  node_hash hasher() const;
  node_equal equaler() const;

  dataframe_tracker::const_iterator find(attribute const &attr) const;

  std::pair<iterator, bool> insert_check(attribute_list const &attributes,
                                         insert_commit_data &commit_data);
  iterator insert_commit(attribute_list const &attributes,
                         insert_commit_data const &commit_data);
  void update(attribute_list const &attributes);

  void visit_records(record_visitor visitor) const final override;
  attribute_description const &descriptor() const final override;
};
END_NAMESPACE_CORE

#endif // DATAFRAME_TRACKER_HPP
