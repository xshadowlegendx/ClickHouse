#pragma once

#include <Parsers/IAST.h>
#include <Parsers/ASTQueryWithOnCluster.h>
#include <Access/Common/QuotaDefs.h>


namespace DB
{
class ASTRolesOrUsersSet;


/** CREATE QUOTA [IF NOT EXISTS | OR REPLACE] name
  *      [KEYED BY {none | user_name | ip_address | client_key | client_key, user_name | client_key, ip_address} | NOT KEYED]
  *      [FOR [RANDOMIZED] INTERVAL number {second | minute | hour | day | week | month | quarter | year}
  *       {MAX {{queries | errors | result_rows | result_bytes | read_rows | read_bytes | execution_time} = number} [,...] |
  *        NO LIMITS | TRACKING ONLY} [,...]]
  *      [TO {role [,...] | ALL | ALL EXCEPT role [,...]}]
  *
  * ALTER QUOTA [IF EXISTS] name
  *      [RENAME TO new_name]
  *      [KEYED BY {none | user_name | ip_address | client_key | client_key, user_name | client_key, ip_address} | NOT KEYED]
  *      [FOR [RANDOMIZED] INTERVAL number {second | minute | hour | day | week | month | quarter | year}
  *       {MAX {{queries | errors | result_rows | result_bytes | read_rows | read_bytes | execution_time} = number} [,...] |
  *        NO LIMITS | TRACKING ONLY} [,...]]
  *      [TO {role [,...] | ALL | ALL EXCEPT role [,...]}]
  */
class ASTCreateQuotaQuery : public IAST, public ASTQueryWithOnCluster
{
public:
    bool alter = false;
    bool attach = false;

    bool if_exists = false;
    bool if_not_exists = false;
    bool or_replace = false;

    Strings names;
    String new_name;
    std::optional<QuotaKeyType> key_type;
    String storage_name;

    struct Limits
    {
        std::optional<QuotaValue> max[static_cast<size_t>(QuotaType::MAX)];
        bool drop = false;
        std::chrono::seconds duration = std::chrono::seconds::zero();
        bool randomize_interval = false;
    };
    std::vector<Limits> all_limits;

    std::shared_ptr<ASTRolesOrUsersSet> roles;

    String getID(char) const override;
    ASTPtr clone() const override;
    void formatImpl(WriteBuffer & ostr, const FormatSettings & settings, FormatState &, FormatStateStacked) const override;
    void replaceCurrentUserTag(const String & current_user_name) const;
    ASTPtr getRewrittenASTWithoutOnCluster(const WithoutOnClusterASTRewriteParams &) const override { return removeOnCluster<ASTCreateQuotaQuery>(clone()); }

    QueryKind getQueryKind() const override { return QueryKind::Create; }
};
}
