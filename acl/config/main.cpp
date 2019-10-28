#include "acl_cpp/lib_acl.hpp"
#include "lib_acl.h"


static int var_cfg_my_port;

static ACL_CFG_INT_TABLE __conf_int_tab[] = {
  /* 配置项名称, 配置项缺省值, 存储配置项值的地址, 保留字, 保留字 */
  { "my_port", 8080, &var_cfg_my_port, 0, 0 },
  { 0, 0 , 0, 0, 0 }
};

static char *var_cfg_my_addr;
static char *var_cfg_my_list;

static ACL_CFG_STR_TABLE __conf_str_tab[] = {
  /* 配置项名称, 配置项缺省值, 存储配置项值的地址 */
  { "my_addr", "192.168.0.1", &var_cfg_my_addr },
  { "my_list", "www.test.com", &var_cfg_my_list },
  { 0, 0, 0 }
};

static int var_cfg_my_check;

static ACL_CFG_BOOL_TABLE __conf_bool_tab[] = {
  /* 配置项名称, 配置项缺省值, 存储配置项值的地址 */
  { "my_check", 0, &var_cfg_my_check },
  { 0, 0, 0 }
};

int main(/*int argc, char* argv[]*/)
{
  acl::acl_cpp_init();
	acl::log::stdout_open(true);  
  ACL_XINETD_CFG_PARSER *cfg;  // 配置解析对象

  cfg = acl_xinetd_cfg_load("test.cfg");  // 读取并解析配置文件
  acl_xinetd_params_int_table(cfg, __conf_int_tab);  // 读取所有 int 类型的配置项
  acl_xinetd_params_str_table(cfg, __conf_str_tab);  // 读取所有字符串类型的配置项
  acl_xinetd_params_bool_table(cfg, __conf_bool_tab);  // 读取所有 bool 型的配置项

  auto size = acl_xinetd_cfg_size(cfg);
  logger("size : %d\n", size);
  acl_xinetd_cfg_free(cfg);  // 释放内存
  return 0;
}