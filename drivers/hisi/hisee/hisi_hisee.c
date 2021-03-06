#include <asm/compiler.h>
#include <linux/compiler.h>
#include <linux/fd.h>
#include <linux/tty.h>
#include <linux/fs.h>
#include <linux/fcntl.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/syscalls.h>
#include <linux/device.h>
#include <linux/dma-mapping.h>
#include <linux/semaphore.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/types.h>
#include <linux/io.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_reserved_mem.h>
#include <linux/of_device.h>
#include <linux/of_platform.h>
#include <linux/of_gpio.h>
#include <linux/delay.h>
#include <linux/timer.h>
#include <linux/atomic.h>
#include <linux/notifier.h>
#include <linux/printk.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/interrupt.h>
#include <linux/hisi/ipc_msg.h>
#include <linux/hisi/hisi_rproc.h>
#include <linux/hisi/kirin_partition.h>
#include <linux/clk.h>
#include <linux/mm.h>
#include "soc_acpu_baseaddr_interface.h"
#include "soc_sctrl_interface.h"
#include "hisi_hisee.h"
#include "hisi_hisee_power.h"
#include "hisi_hisee_fs.h"
#include "hisi_hisee_upgrade.h"
#include "hisi_hisee_chip_test.h"
#include "hisi_hisee_autotest.h"

#define DSM_NFC_HISEE_COS_IMAGE_UPGRADE_ERROR_NO ((long)923002015)
#define DSM_NFC_HISEE_POWER_ON_OFF_ERROR_NO ((long)923002016)
#define DSM_NFC_HISEE_APDU_COMMAND_OPERATION_ERROR_NO ((long)923002017)

#define HISEE_EFUSE_GROUP_BIT_SIZE    32

#define HISEE_SM_EFUSE_GROUP    (g_hisee_sm_efuse_pos/HISEE_EFUSE_GROUP_BIT_SIZE)
#define HISEE_SM_EFUSE_OFFSET   (g_hisee_sm_efuse_pos%HISEE_EFUSE_GROUP_BIT_SIZE)

hisee_module_data g_hisee_data;
atomic_t g_hisee_errno;
unsigned int g_misc_version = 0;
int g_hisee_partition_byname_find = 0;
unsigned int g_platform_id = 0;

/*
 the bit position for SM flag recorded in efuse
 which is started from 0.
 defualt value is invalid
*/
static u32 g_hisee_sm_efuse_pos = 0xFFFF;

extern int nfc_record_dmd_info(long dmd_no, const char *dmd_info);
extern int get_rpmb_init_status(void);
extern int get_rpmb_key_status(void);
extern int get_efuse_hisee_value(unsigned char *pu8Buffer, unsigned int u32Length, unsigned int timeout);
extern int set_efuse_hisee_value(unsigned char *pu8Buffer, unsigned int u32Length, unsigned int timeout);
extern void register_flash_hisee_otp_fn(int (*fn_ptr)(void));

static hisee_errcode_item_des g_errcode_items_des[] = {
	{HISEE_OK, "no error\n"},
	{HISEE_ERROR, "general error\n"},
	{HISEE_NO_RESOURCES, "general no resources\n"},
	{HISEE_INVALID_PARAMS, "general invalid paramters\n"},
	{HISEE_CMA_DEVICE_INIT_ERROR, "cma device init error\n"},
	{HISEE_IOCTL_NODE_CREATE_ERROR, "ioctl node create error\n"},
	{HISEE_POWER_NODE_CREATE_ERROR, "power node create error\n"},
	{HISEE_THREAD_CREATE_ERROR, "thread create error\n"},
	{HISEE_RPMB_MODULE_INIT_ERROR, "rpmb module init error\n"},
	{HISEE_BULK_CLK_INIT_ERROR, "bulk clk dts node init error\n"},
	{HISEE_BULK_CLK_ENABLE_ERROR, "bulk clk error\n"},

	{HISEE_RPMB_KEY_WRITE_ERROR, "write rpmb key fail\n"},
	{HISEE_RPMB_KEY_READ_ERROR, "read rpmb key fail\n"},
	{HISEE_RPMB_KEY_UNREADY_ERROR, "rpmb key not ready error\n"},
	{HISEE_COS_VERIFICATITON_ERROR, "cos img verify fail\n"},
	{HISEE_IMG_PARTITION_MAGIC_ERROR, "img partition magic error\n"},
	{HISEE_IMG_PARTITION_FILES_ERROR, "img partition files error\n"},
	{HISEE_IMG_SUB_FILE_NAME_ERROR, "sub image invalid error\n"},
	{HISEE_SUB_FILE_SIZE_CHECK_ERROR, "sub file size error\n"},
	{HISEE_SUB_FILE_OFFSET_CHECK_ERROR, "sub file offset error\n"},
	{HISEE_IMG_SUB_FILE_ABSENT_ERROR, "sub image absent error\n"},
	{HISEE_FS_SUB_FILE_ABSENT_ERROR, "sub fs file absent error\n"},

	{HISEE_OPEN_FILE_ERROR, "open file error\n"},
	{HISEE_READ_FILE_ERROR, "read file error\n"},
	{HISEE_WRITE_FILE_ERROR, "write file error\n"},
	{HISEE_CLOSE_FILE_ERROR, "close file error\n"},
	{HISEE_LSEEK_FILE_ERROR, "seek file error\n"},
	{HISEE_OUTOF_RANGE_FILE_ERROR, "out of file threshold error\n"},

	{HISEE_FS_MALLOC_ERROR, "fs partition malloc error\n"},
	{HISEE_FS_PATH_ABSENT_ERROR, "fs partition absent error\n"},
	{HISEE_FS_OPEN_PATH_ERROR, "fs partition open error\n"},
	{HISEE_FS_COUNT_FILES_ERROR, "fs partition calculate files error\n"},
	{HISEE_FS_PATH_LONG_ERROR, "sub fs file path too long error\n"},
	{HISEE_FS_READ_FILE_ERROR, "sub fs file read error\n"},

	{HISEE_POWERCTRL_TIMEOUT_ERROR, "hisee powerctrl timeout error\n"},
	{HISEE_POWERCTRL_NOTIFY_ERROR, "hisee powerctrl notify error\n"},
	{HISEE_POWERCTRL_RETRY_FAILURE_ERROR, "hisee powerctrl retry failure error\n"},
	{HISEE_POWERCTRL_FLOW_ERROR, "hisee powerctrl flow error\n"},

	{HISEE_FIRST_SMC_CMD_ERROR, "the fisrt step in smc transcation error\n"},
	{HISEE_SMC_CMD_TIMEOUT_ERROR, "the smc transcation timeout error\n"},
	{HISEE_SMC_CMD_PROCESS_ERROR, "the smc transcation failure error\n"},

	{HISEE_CHANNEL_TEST_CMD_ERROR, "invalid channel test cmd error\n"},
	{HISEE_CHANNEL_TEST_RESULT_MALLOC_ERROR, "channel test result buf malloc error\n"},
	{HISEE_CHANNEL_TEST_PATH_ABSENT_ERROR, "channel test input file error\n"},
	{HISEE_CHANNEL_TEST_WRITE_RESULT_ERROR, "channel test write result file error\n"},

	{HISEE_GET_HISEE_VALUE_ERROR,"get hisee lcs mode error\n"},
	{HISEE_SET_HISEE_VALUE_ERROR,"set hisee lcs mode error\n"},
	{HISEE_SET_HISEE_STATE_ERROR,"set hisee state error\n"},

	{HISEE_WAIT_READY_TIMEOUT,"wait hisee ready timeout\n"},

	{HISEE_ENCOS_HEAD_INIT_ERROR, "hisee encos head init error\n"},
	{HISEE_COS_IMG_ID_ERROR, "the input cosid error\n"},
	{HISEE_ENCOS_PARTITION_MAGIC_ERROR, "hisee encos magic num error\n"},
	{HISEE_ENCOS_PARTITION_FILES_ERROR, "hisee encos files error\n"},
	{HISEE_ENCOS_PARTITION_SIZE_ERROR, "hisee encos file size error\n"},
	{HISEE_ENCOS_SUBFILE_SIZE_CHECK_ERROR, "hisee encos subfile size error\n"},
	{HISEE_ENCOS_OPEN_FILE_ERROR, "hisee encos open file error\n"},
	{HISEE_ENCOS_FIND_PTN_ERROR, "hisee encos find ptn error\n"},
	{HISEE_ENCOS_LSEEK_FILE_ERROR, "hisee encos lseek error\n"},
	{HISEE_ENCOS_WRITE_FILE_ERROR, "hisee encos write file error\n"},
	{HISEE_ENCOS_READ_FILE_ERROR, "hisee encos read file error\n"},
	{HISEE_ENCOS_ACCESS_FILE_ERROR, "hisee encos access file error\n"},
	{HISEE_ENCOS_CHECK_HEAD_ERROR, "hisee encos check file head error\n"},
	{HISEE_ENCOS_NAME_NULL_ERROR, "hisee encos file name is null\n"},

	{HISEE_OLD_COS_IMAGE_ERROR,"old cos image error\n"}
};

#define MANAFACTOR_CMD_INDEX 2

static hisee_driver_function g_hisee_atf_function_list[] = {
	{"cos_image_upgrade", cos_image_upgrade_func},
	{"hisee_channel_test", hisee_channel_test_func},
	{"hisee_parallel_factory_action", hisee_parallel_manufacture_func},
#ifdef __SLT_FEATURE__
	{"hisee_slt_action", hisee_parallel_total_slt_func},
	{"hisee_check_slt", hisee_read_slt_func},
	{"hisee_slt", hisee_total_slt_func},
#endif
	{NULL, NULL},
};

static hisee_driver_function g_hisee_lpm3_function_list[] = {
	{"hisee_poweron_upgrade", hisee_poweron_upgrade_func},
	{"hisee_poweron_timeout", hisee_poweron_timeout_func},
	{"hisee_poweron", hisee_poweron_booting_func},
	{"hisee_poweroff", hisee_poweroff_func},
	{NULL, NULL},
};

noinline int atfd_hisee_smc(u64 function_id, u64 arg0, u64 arg1, u64 arg2)
{
    asm volatile(
            __asmeq("%0", "x0")
            __asmeq("%1", "x1")
            __asmeq("%2", "x2")
            __asmeq("%3", "x3")
            "smc    #0\n"
        : "+r" (function_id)
        : "r" (arg0), "r" (arg1), "r" (arg2));

    return (int)function_id;
} /*lint !e715*/



int get_hisee_lcs_mode(unsigned int *mode)
{
	int ret;
	u32 hisee_value[2] = {0};

	if(HISEE_SM_EFUSE_GROUP >= 2) {
		pr_err("sm_flag_pos invalid\n");
		set_errno_and_return(HISEE_GET_HISEE_VALUE_ERROR);
	}

	ret = get_efuse_hisee_value((unsigned char *)hisee_value, 8, 1000);
	if (HISEE_OK != ret) {
		pr_err("%s() get_efuse_hisee_value failed,ret=%d\n", __func__, ret);
		set_errno_and_return(HISEE_GET_HISEE_VALUE_ERROR);
	}
	*mode = ((hisee_value[HISEE_SM_EFUSE_GROUP] >> HISEE_SM_EFUSE_OFFSET) & 1) ?\
		 HISEE_SM_MODE_MAGIC : HISEE_DM_MODE_MAGIC;
	return ret;
}

int set_hisee_lcs_sm_efuse(void)
{
	int ret;
	u32 hisee_value[2] = {0};

	if (HISEE_SM_EFUSE_GROUP >= 2) {
		pr_err("sm_flag_pos invalid\n");
		set_errno_and_return(HISEE_SET_HISEE_VALUE_ERROR);
	}

	hisee_value[HISEE_SM_EFUSE_GROUP] = (((u32)0x1) << HISEE_SM_EFUSE_OFFSET);
	ret = set_efuse_hisee_value((unsigned char *)hisee_value, 8, 1000);
	if (HISEE_OK != ret) {
		pr_err("%s() set_efuse_hisee_value failed,ret=%d\n", __func__, ret);
		set_errno_and_return(HISEE_SET_HISEE_VALUE_ERROR);
	}
	check_and_print_result();
	return ret;
}

static void record_hisee_log_by_dmd(long dmd_errno, int hisee_errno)
{
#ifdef CONFIG_PN547_NFC_64
	int ret;
#endif
	char buff[HISEE_CMD_NAME_LEN] = {0};
	int i;

	memset(buff, 0, (unsigned long)HISEE_CMD_NAME_LEN);
	for (i = 0; i < ARRAY_SIZE(g_errcode_items_des); i++) {/*lint !e846 !e514 !e866 !e30 !e84 !e574 !e737*/
		if (hisee_errno == g_errcode_items_des[i].err_code) {
			break;
		}
	}
	if ((unsigned long)i == ARRAY_SIZE(g_errcode_items_des)) {/*lint !e846 !e514 !e866 !e30 !e84 !e574 !e737 !e571*/
		pr_err("%s(): can't find errcode=%d definition\n", __func__, hisee_errno);
		return;
	}

	snprintf(buff, (unsigned long)8, "%d,", hisee_errno);
	strncat(buff, g_errcode_items_des[i].err_description, (unsigned long)((HISEE_ERROR_DESCRIPTION_MAX - 1) - strlen(buff)));/*lint !e662*/

#ifdef CONFIG_PN547_NFC_64
	ret = nfc_record_dmd_info(dmd_errno, (const char *)buff);
	if (0 != ret)
		pr_err("%s(): nfc_record_dmd_for_hisee return %d failed\n", __func__, ret);
#endif

	return;
}

/*
 * hisi_hisee_active - handle hisee request from ATF
 * if wait_sem_timeout is not zero, means there has been a timeout
 * before this IPI happens, then only decrease it;
 * another case:if smc_cmd_running is zero, there nothing need to do;
 * otherwise, do up() a semaphore
 */
void hisi_hisee_active(void)
{
/*
	pr_err("%s() sem_timeout=%d smc_cmd_running=%d, count=%d\n", __func__,
		atomic_read(&g_hisee_data.wait_sem_timeout), g_hisee_data.smc_cmd_running, g_hisee_data.atf_sem.count);

	if (atomic_sub_return(1, &(g_hisee_data.wait_sem_timeout)) >= 0) {
		return;
	} else {
		atomic_set(&(g_hisee_data.wait_sem_timeout), 0);
	}
*/
	if (g_hisee_data.smc_cmd_running) {
		up(&(g_hisee_data.atf_sem));
	}
	return;
}
EXPORT_SYMBOL(hisi_hisee_active);

void set_message_header(atf_message_header *header, unsigned int cmd_type)
{
	header->ack = 0;
	header->cmd = cmd_type;
	header->test_result_phy = 0;
	header->test_result_size = 0;
	return;
}

int send_smc_process(atf_message_header *p_message_header, phys_addr_t phy_addr, unsigned int size,
							unsigned int timeout, se_smc_cmd smc_cmd)
{
	int ret = HISEE_OK;
	long local_jiffies;

	mutex_lock(&g_hisee_data.hisee_mutex);
	g_hisee_data.smc_cmd_running = 1;

	if (CMD_HISEE_CHANNEL_TEST != smc_cmd)
		ret = atfd_hisee_smc((u64)HISEE_FN_MAIN_SERVICE_CMD, (u64)smc_cmd, (u64)phy_addr, (u64)size);
	else
		ret = atfd_hisee_smc((u64)HISEE_FN_CHANNEL_TEST_CMD, (u64)smc_cmd, (u64)phy_addr, (u64)size);
	if (ret != HISEE_OK) {
		pr_err("%s(): atfd_hisee_smc failed, ret=%d\n", __func__, ret);
		g_hisee_data.smc_cmd_running = 0;
		mutex_unlock(&g_hisee_data.hisee_mutex);
		set_errno_and_return(HISEE_FIRST_SMC_CMD_ERROR);
	}

	/*notice! while starting a new smc message, no need to set wait_sem_timeout to zero.
	 *because this flag only set in timeout scenario, so only clear in IPI context.
	 *If the wait_sem_timeout is set in the second smc message, there may be a uncorrection:
	 *the fisrt IPI interrupt should make the semaphore availble, then the second acquire semaphore
	 *success directly.
	*/
	if (CMD_HISEE_CHANNEL_TEST != smc_cmd)
		local_jiffies = msecs_to_jiffies(timeout);
	else
		local_jiffies = MAX_SCHEDULE_TIMEOUT;
	ret = down_timeout(&(g_hisee_data.atf_sem), local_jiffies);
	if (-ETIME == ret) {
		ret = HISEE_SMC_CMD_TIMEOUT_ERROR;
	} else {
		if (!(p_message_header->cmd == smc_cmd &&
			p_message_header->ack == HISEE_ATF_ACK_SUCCESS))
			ret = HISEE_SMC_CMD_PROCESS_ERROR;
		else
			ret = HISEE_OK;
		/*ret = get_hisee_lcs_mode(&hisee_lcs_mode);
		if (HISEE_OK == ret) {
			if (CMD_FACTORY_APDU_TEST == smc_cmd &&
				HISEE_SM_MODE_MAGIC == hisee_lcs_mode) {
				if (!(p_message_header->cmd == smc_cmd &&
					p_message_header->ack == HISEE_ATF_ACK_FAILURE)) {
					ret = HISEE_SMC_CMD_PROCESS_ERROR;
				}
			} else {
				if (!(p_message_header->cmd == smc_cmd &&
					p_message_header->ack == HISEE_ATF_ACK_SUCCESS)) {
					ret = HISEE_SMC_CMD_PROCESS_ERROR;
				}
			}
		} else
			ret = HISEE_SMC_CMD_PROCESS_ERROR;*/
	}

	pr_err("%s() ret=%d\n", __func__, ret);
	g_hisee_data.smc_cmd_running = 0;
	mutex_unlock(&g_hisee_data.hisee_mutex);
	return ret;
}

static int write_apdu_command_func (char *apdu_buf, unsigned int apdu_len)
{
	atf_message_header *p_message_header;
	int ret = HISEE_OK;
	int image_size;

	if (NULL == g_hisee_data.apdu_command_buff_virt) {
		g_hisee_data.apdu_command_buff_virt = (void *)dma_alloc_coherent(g_hisee_data.cma_device, (u64)(SIZE_1K * 4),
											&(g_hisee_data.apdu_command_buff_phy), GFP_KERNEL);
	}
	if (NULL == g_hisee_data.apdu_command_buff_virt) {
		pr_err("%s(): dma_alloc_coherent failed\n", __func__);
		set_errno_and_return(HISEE_NO_RESOURCES);
	}
	memset(g_hisee_data.apdu_command_buff_virt, 0, (u64)(SIZE_1K * 4));
	p_message_header = (atf_message_header *)g_hisee_data.apdu_command_buff_virt; /*lint !e826*/
	set_message_header(p_message_header, CMD_APDU_RAWDATA);

	apdu_len = (apdu_len > HISEE_APDU_DATA_LEN_MAX) ?  HISEE_APDU_DATA_LEN_MAX : apdu_len;
	memcpy(g_hisee_data.apdu_command_buff_virt + HISEE_ATF_MESSAGE_HEADER_LEN, (void *)apdu_buf, (u64)(u32)apdu_len);/*lint !e124*/
	image_size = HISEE_ATF_MESSAGE_HEADER_LEN + apdu_len;
	p_message_header->test_result_phy = (u32)(g_hisee_data.apdu_command_buff_phy + (SIZE_1K * 2));
	p_message_header->test_result_size = (unsigned int)SIZE_1K;
	ret = send_smc_process(p_message_header, g_hisee_data.apdu_command_buff_phy, (u32)image_size,
							HISEE_ATF_GENERAL_TIMEOUT, CMD_APDU_RAWDATA);

	if (HISEE_OK == ret && p_message_header->test_result_size <= HISEE_APDU_DATA_LEN_MAX) {
		memcpy(g_hisee_data.apdu_ack.ack_buf, (g_hisee_data.apdu_command_buff_virt + (SIZE_1K * 2)), (u64)p_message_header->test_result_size);
		g_hisee_data.apdu_ack.ack_len = p_message_header->test_result_size;
	} else {
		g_hisee_data.apdu_ack.ack_len = 0;
		pr_err("%s(): p_message_header->test_result_size=%x failed\n", __func__, p_message_header->test_result_size);
	}

	check_and_print_result();
	set_errno_and_return(ret);
}/*lint !e715*/

int send_apdu_cmd(int type)
{
	int ret;
	/*set key cmd */
	unsigned char cmd0[21] = {	0xF0, 0x10, 0x00, 0x00, \
								0x10, 0x01, 0x23, 0x45, \
								0x67, 0x89, 0xab, 0xcd, \
								0xef, 0xfe, 0xdc, 0xba, \
								0x98, 0x76, 0x54, 0x32, \
								0x10};
	unsigned char cmd1[5] = {0xF0,0xd8, 0x00,0x00,0x00};
	/* delete test applet cmd */
	unsigned char cmd2[5] = {0x00, 0xa4, 0x04, 0x00, 0x00};
	unsigned char cmd3[12] = {0x80, 0xe4, 0x00, 0x80,0x07, \
							0x4f, 0x05, 0x12, 0x34, 0x56, \
							0x78, 0x90};
	unsigned char *sel_cmd[2];
	unsigned int sel_cmd_len[2];

	if (HISEE_SET_KEY == type) {
		sel_cmd[0] = cmd0;
		sel_cmd[1] = cmd1;
		sel_cmd_len[0] = 21;
		sel_cmd_len[1] = 5;
	}
	else if (HISEE_DEL_TEST_APPLET == type) {
		sel_cmd[0] = cmd2;
		sel_cmd[1] = cmd3;
		sel_cmd_len[0] = 5;
		sel_cmd_len[1] = 12;
	}
	else {
		return HISEE_INVALID_PARAMS;
	}

	/* send apdu key command */
	ret = write_apdu_command_func((char *)sel_cmd[0], sel_cmd_len[0]);
	if (HISEE_OK != ret) {
		pr_err("%s()  apdu0 failed,ret=%d\n", __func__, ret);
		return ret;
	}
	hisee_mdelay(DELAY_BETWEEN_STEPS);
	ret = write_apdu_command_func((char *)sel_cmd[1], sel_cmd_len[1]);
	if (HISEE_OK != ret) {
		pr_err("%s()  apdu1 failed,ret=%d\n", __func__, ret);
		return ret;
	}
	hisee_mdelay(DELAY_BETWEEN_STEPS);

	return ret;
}

static int load_cosimg_appdata_ddr(void)
{
	char *buff_virt;
	unsigned int i;
	unsigned int rpmb_cos;
	phys_addr_t buff_phy = 0;
	atf_message_header *p_message_header;
	int ret = HISEE_OK;
	int image_size = 0;

	buff_virt = (void *)dma_alloc_coherent(g_hisee_data.cma_device, SIZE_1K * 4,
											&buff_phy, GFP_KERNEL);
	if (buff_virt == NULL) {
		pr_err("%s(): dma_alloc_coherent failed\n", __func__);
		set_errno_and_return(HISEE_NO_RESOURCES);
	}
	memset(buff_virt, 0, SIZE_1K * 4);
	p_message_header = (atf_message_header *)buff_virt;
	set_message_header(p_message_header, CMD_PRESAVE_COS_APPDATA);
	image_size = HISEE_ATF_MESSAGE_HEADER_LEN;
	rpmb_cos = g_hisee_data.hisee_img_head.rpmb_cos_cnt;
	if (0 == rpmb_cos)
		rpmb_cos = HISEE_MAX_RPMB_COS_NUMBER;
	for (i = 0; i < rpmb_cos; i++) {
		p_message_header->ack = i;
		ret = send_smc_process(p_message_header, buff_phy, image_size,
							HISEE_ATF_COS_APPDATA_TIMEOUT, CMD_PRESAVE_COS_APPDATA);
	}
	dma_free_coherent(g_hisee_data.cma_device, (unsigned long)(SIZE_1K * 4), buff_virt, buff_phy);
	check_and_print_result();
	set_errno_and_return(ret);
}

static int hisee_wait_partition_ready(void)
{
        char fullpath[128] = {0};
        int  retry = 10;
        unsigned int  timeout = 500;
        int  ret;

        ret = flash_find_ptn(HISEE_IMAGE_PARTITION_NAME, fullpath);
        if (0 != ret) {
                pr_err("%s():flash_find_ptn fail\n", __func__);
                BUG_ON(1);/*lint !e730*/
        }
        do {
                if(0==sys_access(fullpath,0)) {
                        break;
                }
                msleep(timeout);
                retry--;
        } while(retry > 0);

	if (retry <= 0)
	      return HISEE_ERROR;

#ifdef HISEE_SUPPORT_MULTI_COS
	if (g_platform_id == HISEE_PLATFORM_3670_CS) {
		ret = hisee_parse_img_header((char *)&(g_hisee_data.hisee_img_head));
		if (HISEE_OK != ret) {
			return HISEE_ERROR;
		}
		retry = 10;
		do {
			ret = hisee_encos_header_init();
			if (HISEE_OK == ret) {
				break;
			}
			msleep(timeout);
			retry--;
		} while (retry > 0);
		if (0 == retry) {
			pr_err("%s():hisee_encos_header_init failed, ret=%d\n", __func__, ret);
			BUG();
		}
	}
#endif
        return HISEE_OK;
}

#ifdef HISEE_SUPPORT_MULTI_COS
int load_encos_image_ddr(void)
{
	char *buff_virt;
	unsigned int i;
	unsigned int emmc_cos;
	phys_addr_t buff_phy = 0;
	atf_message_header *p_message_header;
	int ret = HISEE_OK;
	unsigned int image_size;

	buff_virt = (void *)dma_alloc_coherent(g_hisee_data.cma_device, (unsigned long)HISEE_SHARE_BUFF_SIZE,
											&buff_phy, GFP_KERNEL);
	if (buff_virt == NULL) {
		pr_err("%s(): dma_alloc_coherent failed\n", __func__);
		set_errno_and_return(HISEE_NO_RESOURCES);
	}

	emmc_cos = g_hisee_data.hisee_img_head.emmc_cos_cnt;
	if (0 == emmc_cos)
		emmc_cos = HISEE_MAX_EMMC_COS_NUMBER;
	for (i = 0; i < emmc_cos; i++) {
		if (g_hisee_encos_header.file[i].name[0] != 0) {
			memset(buff_virt, 0, (unsigned long)HISEE_SHARE_BUFF_SIZE);
			p_message_header = (atf_message_header *)buff_virt;/*lint !e826*/
			set_message_header(p_message_header, CMD_LOAD_ENCOS_DATA);
			p_message_header->ack = i + COS_IMG_ID_3;
			ret = hisee_encos_read((buff_virt + HISEE_ATF_MESSAGE_HEADER_LEN), g_hisee_encos_header.file[i].size,
						i + COS_IMG_ID_3);
			if (ret < HISEE_OK) {
				pr_err("%s(): hisee_encos_read fail\n", __func__);
				goto out;
			}
			image_size = (g_hisee_encos_header.file[i].size + HISEE_ATF_MESSAGE_HEADER_LEN);
			ret = send_smc_process(p_message_header, buff_phy, image_size,
					HISEE_ATF_COS_TIMEOUT, CMD_LOAD_ENCOS_DATA);
			if (ret < HISEE_OK) {
				pr_err("%s(): hisee_encos_read fail\n", __func__);
				goto out;
			}
		}
	}
out:
	dma_free_coherent(g_hisee_data.cma_device, (unsigned long)HISEE_SHARE_BUFF_SIZE, buff_virt, buff_phy);
	check_and_print_result();
	set_errno_and_return(ret);
}/*lint !e715*/

static int hisee_preload_encos_img(void)
{
	int ret = HISEE_OK;

	if (g_platform_id == HISEE_PLATFORM_3670_CS) {
		ret = load_encos_image_ddr();
		if (ret != HISEE_OK) {
			pr_err("hisee encos read failed!\n");
		}
	}
	return ret;
}
#endif

/*lint -e438 -esym(438,*) */
int hisee_lpmcu_send(rproc_msg_t msg_0, rproc_msg_t msg_1)
{
	int retry;
	int ret = HISEE_OK;
	rproc_id_t rproc_id = HISI_RPROC_LPM3_MBX27;
	rproc_msg_t tx_buffer[HISEE_IPC_TXBUF_SIZE] = {0};
	rproc_msg_t ack_buffer[2] = {0};

	tx_buffer[0] = IPC_CMD(OBJ_AP, OBJ_LPM3, CMD_NOTIFY, 0);
	tx_buffer[1] = msg_0;
	tx_buffer[2] = msg_1;
	retry = 3;

	do {
		ret = RPROC_SYNC_SEND(rproc_id, tx_buffer, HISEE_IPC_TXBUF_SIZE, ack_buffer, 2);
		if ((0 == ret) && (HISEE_LPM3_CMD == ack_buffer[0]) && (HISEE_LPM3_ACK_MAGIC == ack_buffer[1])) {
			/* the send is reponsed by the remote process, break out */
			ret = HISEE_OK;
			break;
		}
		else if (-ETIMEOUT == ret) {
			/*the timeout will print out, below message to tell it's normal*/
		    retry--;
			ret = HISEE_POWERCTRL_TIMEOUT_ERROR;
			pr_err("%s(): the ack of sending ipc is timeout.\n",  __func__);
			continue;
		} else {
			pr_err("%s(): send ipc failed\n", __func__);
			ret = HISEE_POWERCTRL_NOTIFY_ERROR;
			retry = 0;
			break;
		}
	} while (retry);

	if (0 == retry) {
		pr_err("%s(): send ipc with retry still failed\n", __func__);
		ret = HISEE_POWERCTRL_RETRY_FAILURE_ERROR;
	}

	set_errno_and_return(ret);
}
/*lint +e438 +esym(438,*) */

static int set_lpmcu_nfc_irq(void)
{
	struct device_node *pn547_nfc_dev;
	int irq_gpio;

	pn547_nfc_dev = of_find_compatible_node(NULL, NULL, "hisilicon,pn547_nfc");
	if (!pn547_nfc_dev) {
		pr_err("%s pn547_nfc not found\n", __func__);
		return hisee_lpmcu_send(DISABLE_NFC_IRQ, (unsigned int)0);
	}

	irq_gpio  = of_get_named_gpio(pn547_nfc_dev, "pn547,irq", 0);
	if (!gpio_is_valid(irq_gpio)) {
		pr_err("%s pn547,irq not found\n", __func__);
		return hisee_lpmcu_send(DISABLE_NFC_IRQ, (unsigned int)0);
	}

	return hisee_lpmcu_send(SET_NFC_IRQ, (unsigned int)irq_gpio);
}

static int rpmb_ready_body(void *arg)
{
	unsigned int timeout = 500;
	int retry = 10;
	int ret = HISEE_OK;
	int find_part_res = HISEE_OK;
	unsigned int upgrade_run_flg = 0;
	unsigned int value;
	unsigned int hisee_lcs_mode = 0;
	char cos_id;
	char cosid_buf[4] = {' ', ' ', ' ', '\0'}; /* buf[0] is bank, buf[1] is cos id, buf[2] is vote id */

	/*the task sleep 16 second until the kernel init completed */
	//msleep(16000);
	/*poll rpmb module ready status about 90 seconds*/
	do {
		if (get_rpmb_init_status()) {
			g_hisee_data.rpmb_is_ready = 1;
			break;
		}
		msleep(timeout);
		retry--;
	} while (retry > 0);

	if (retry > 0 && 1 == g_hisee_partition_byname_find) {
		find_part_res = hisee_wait_partition_ready();
	}

	/* set lcs mode for lpmcu */
	if (HISEE_OK != get_hisee_lcs_mode(&hisee_lcs_mode)) {
		ret = HISEE_ERROR;
		pr_err("%s() get_hisee_lcs_mode failed,ret=%d\n", __func__, ret);
		set_errno_and_return(ret);
	}
	value = *(volatile unsigned int *)(HISEE_STATE_ADDR); /*lint !e747*/
	if (HISEE_DM_MODE_MAGIC == hisee_lcs_mode)
	{
		value |= BIT(HISEE_LCS_DM_BIT);
	} else {
		value &= (~BIT(HISEE_LCS_DM_BIT));
	}
	*(volatile unsigned int *)(HISEE_STATE_ADDR) = value; /*lint !e747*/

	if (0 == retry || HISEE_OK != find_part_res) {
		pr_err("%s BUG_ON\n", __func__);
		BUG_ON(1);
	} else {
		access_hisee_image_partition((char *)&upgrade_run_flg, COS_UPGRADE_RUN_READ_TYPE);
		if (HISEE_COS_UPGRADE_RUNNING_FLG == upgrade_run_flg) {
			/*If last time upgrade fail, we should get the last cosid for retrying upgrade process.*/
			access_hisee_image_partition((char *)&cos_id, SW_COSID_RUN_READ_TYPE);
			if (get_rpmb_key_status()) { /*key ready*/
				cosid_buf[1] = '0' + cos_id;
				cosid_buf[2] = '0' + (char)COS_PROCESS_UPGRADE; 
				wake_lock(&g_hisee_data.wake_lock);
				hisee_poweron_upgrade_func(cosid_buf, 0);
				hisee_mdelay(200);
				ret = cos_image_upgrade_func(cosid_buf, 0);
				wake_unlock(&g_hisee_data.wake_lock);
			}
		} else {
			ret = load_cosimg_appdata_ddr();
			if (ret != HISEE_OK) {
				pr_err("hisee rpmb cos read failed!\n");
			}
#ifdef HISEE_SUPPORT_MULTI_COS
			ret = hisee_preload_encos_img();
			if (HISEE_OK != ret) {
				pr_err("hisee preload encos failed!\n");
			}
#endif
		}
	}

	check_and_print_result();
	set_errno_and_return(ret);
}

static int flash_hisee_debug_switchs(void)
{
    int ret;

    ret = hisee_poweron_booting_func(NULL, 0);
    if (HISEE_OK != ret) {
        set_errno_and_return(ret);
    }

    ret = wait_hisee_ready(HISEE_STATE_COS_READY, 800);
    if (HISEE_OK == ret) {
        ret = write_hisee_otp_value(OTP1_IMG_TYPE);
        if (HISEE_OK != ret)
            pr_err("%s() write_hisee_otp_value failed,ret=%d\n", __func__, ret);
    }

    hisee_mdelay(DELAY_BETWEEN_STEPS);
    (void)hisee_poweroff_func(NULL, (int)HISEE_PWROFF_LOCK);

    check_and_print_result();
    set_errno_and_return(ret);
}

static void show_hisee_module_status(char *buff)
{
	char counter_value[12] = {0};
	char *index_name = "rpmb_is_ready=";
	snprintf(counter_value, sizeof(counter_value), "%d\n", g_hisee_data.rpmb_is_ready);
	strncpy(buff, index_name, strlen(index_name));
	strncpy(buff + strlen(buff), counter_value, strlen(counter_value));

	memset(counter_value, 0, 12);
	index_name = "smc_cmd_running=";
	snprintf(counter_value, sizeof(counter_value), "%d\n", g_hisee_data.smc_cmd_running);
	strncpy(buff + strlen(buff), index_name, strlen(index_name));
	strncpy(buff + strlen(buff), counter_value, strlen(counter_value));

	memset(counter_value, 0, 12);
	index_name = "img_header_is_parse=";
	snprintf(counter_value, sizeof(counter_value), "%d\n", g_hisee_data.img_header_is_parse);
	strncpy(buff + strlen(buff), index_name, strlen(index_name));
	strncpy(buff + strlen(buff), counter_value, strlen(counter_value));

	return;
}

/** read the powerctrl_cmd execute status
 * @buf: output, save the ioctl_cmd execute status, the size not less than 160 bytes
 */
static ssize_t hisee_powerctrl_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	int ret = 0;
	int status = 0;
	char buff[HISEE_ERROR_DESCRIPTION_MAX] = {0};

	if (NULL == buf) {
		pr_err("%s buf paramters is null\n", __func__);
		set_errno_and_return(HISEE_INVALID_PARAMS);
	}
	memset(buff, 0, HISEE_ERROR_DESCRIPTION_MAX);
	status = hisee_get_power_status();
	switch (status) {
	case HISEE_POWER_STATUS_ON:
		strncat(buff, "1", HISEE_ERROR_DESCRIPTION_MAX - 1 - strlen(buff));
		break;
	case HISEE_POWER_STATUS_OFF:
		strncat(buff, "2", HISEE_ERROR_DESCRIPTION_MAX - 1 - strlen(buff));
		break;
	default:
		pr_err("%s(): invalid powerctrl status\n", __func__);
		strncpy(buf, "0", sizeof("0"));
		return strlen(buf);
	}

	memcpy(buf, buff, HISEE_ERROR_DESCRIPTION_MAX);
	pr_err("%s(): success.\n", __func__);
	return strlen(buf);
}/*lint !e715*/


static int hisee_powerctrl_store_params_check(const char *buf, size_t count, unsigned int *cmd_index)
{
	char *ptr_name = NULL;
	unsigned int i = 0;
	int ret;

	if (NULL == buf || (strlen(buf) >= HISEE_CMD_NAME_LEN)) {
		pr_err("%s buf paramters is invalid\n", __func__);
		set_errno_and_return(HISEE_INVALID_PARAMS);
	}
	if (count <= 0 || (count >= HISEE_CMD_NAME_LEN)) {
		pr_err("%s count paramters is invalid\n", __func__);
		set_errno_and_return(HISEE_INVALID_PARAMS);
	}

	for (ptr_name = g_hisee_lpm3_function_list[i].function_name; ptr_name != NULL;) {
		if (!strncmp(buf, ptr_name, strlen(ptr_name))) {
			*cmd_index = i;
			break;
		}
		i++;
		ptr_name = g_hisee_lpm3_function_list[i].function_name;
	}
	if (!ptr_name) {
		pr_err("%s cmd=%s invalid\n", __func__, buf);
		set_errno_and_return(HISEE_INVALID_PARAMS);
	}

	return HISEE_OK;
}

/** write the powerctrl_cmd to execute
 * @buf: input, save the powerctrl_cmd string should be execute
 */
static ssize_t hisee_powerctrl_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
	int ret;
	unsigned int cmd_index = 0;
	unsigned int upgrade_run_flg;
	unsigned int hisee_lcs_mode = 0;

	ret = hisee_powerctrl_store_params_check(buf, count, &cmd_index);
	if (HISEE_OK != ret) {
		pr_err("%s() hisee_powerctrl_store_params_check failed,ret=%d\n", __func__, ret);
		set_errno_and_return(HISEE_INVALID_PARAMS);
	}

	ret = get_hisee_lcs_mode(&hisee_lcs_mode);
	if (HISEE_OK != ret) {
		pr_err("%s() get_hisee_lcs_mode failed,ret=%d\n", __func__, ret);
		if (0 == cmd_index) set_errno_and_return(HISEE_INVALID_PARAMS);
	}
	if ((unsigned int)HISEE_SM_MODE_MAGIC == hisee_lcs_mode && (0 == cmd_index)) {/*to be improved*/
		upgrade_run_flg = HISEE_COS_UPGRADE_RUNNING_FLG;
		access_hisee_image_partition((char *)&upgrade_run_flg, COS_UPGRADE_RUN_WRITE_TYPE);
	}
	if (!g_hisee_data.rpmb_is_ready) {
		pr_err("%s rpmb is not ready now\n", __func__);
		record_hisee_log_by_dmd(DSM_NFC_HISEE_POWER_ON_OFF_ERROR_NO, HISEE_RPMB_MODULE_INIT_ERROR);
		set_errno_and_return(HISEE_RPMB_MODULE_INIT_ERROR);
	}
	ret = g_hisee_lpm3_function_list[cmd_index].function_ptr((void *)(buf + strlen(g_hisee_lpm3_function_list[cmd_index].function_name)), (int)(long)dev);
	if (ret !=  HISEE_OK) {
		pr_err("%s powerctrl_cmd:%s failed, retcode=%d\n", __func__, g_hisee_lpm3_function_list[cmd_index].function_name, ret);
		record_hisee_log_by_dmd(DSM_NFC_HISEE_POWER_ON_OFF_ERROR_NO, ret);
		return HISEE_INVALID_PARAMS;
	} else {
		pr_err("%s :%s success\n", __func__, g_hisee_lpm3_function_list[cmd_index].function_name);
		if ((unsigned int)HISEE_SM_MODE_MAGIC == hisee_lcs_mode && (0 == cmd_index)) {
			upgrade_run_flg = 0;
			access_hisee_image_partition((char *)&upgrade_run_flg, COS_UPGRADE_RUN_WRITE_TYPE);
		}
		return count;
	}
}

/** read the ioctl_cmd execute status
 * @buf: output, save the ioctl_cmd execute status, the size should less than 64 bytes
 */
static ssize_t hisee_ioctl_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	int err_code;
	int ret = 0;
	char buff[HISEE_CMD_NAME_LEN] = {0};
	unsigned int i;

	if (NULL == buf) {
		pr_err("%s buf paramters is null\n", __func__);
		set_errno_and_return(HISEE_INVALID_PARAMS);
	}
	memset(buff, 0, HISEE_CMD_NAME_LEN);
	err_code = atomic_read(&g_hisee_errno);
	for (i = 0; i < ARRAY_SIZE(g_errcode_items_des); i++) {
		if (err_code == g_errcode_items_des[i].err_code) {
			break;
		}
	}
	if (i == ARRAY_SIZE(g_errcode_items_des)) {
		pr_err("%s(): can't find errcode=%d definition\n", __func__, err_code);
		strncpy(buf, "-1,failed!", sizeof("-1,failed!"));
		return strlen(buf);
	}
	if (HISEE_OK == err_code) {
		if (HISEE_FACTORY_TEST_NORUNNING == g_hisee_data.factory_test_state) {
			snprintf(buff, 8, "%d,", 2);
			strncat(buff, "no running", (unsigned long)(HISEE_ERROR_DESCRIPTION_MAX - 1 - strlen(buff)));
		} else if (HISEE_FACTORY_TEST_RUNNING == g_hisee_data.factory_test_state) {
			snprintf(buff, 8, "%d,", 1);
			strncat(buff, "running", (unsigned long)(HISEE_ERROR_DESCRIPTION_MAX - 1 - strlen(buff)));
		} else if (HISEE_FACTORY_TEST_SUCCESS == g_hisee_data.factory_test_state) {
			snprintf(buff, 8, "%d,", 0);
			strncat(buff, "no error", (unsigned long)(HISEE_ERROR_DESCRIPTION_MAX - 1 - strlen(buff)));
		}
	} else {
		snprintf(buff, 8, "%d,", err_code);
		strncat(buff, g_errcode_items_des[i].err_description, (unsigned long)((HISEE_ERROR_DESCRIPTION_MAX - 1) - strlen(buff)));/* lint !e834 */
	}
	memcpy(buf, buff, HISEE_ERROR_DESCRIPTION_MAX);/*lint !e747*/

	memset(buff, 0, (unsigned long)HISEE_CMD_NAME_LEN);/*lint !e747*/
	show_hisee_module_status(buff);
	pr_err("%s\n", buff);

	return strlen(buf);
}

/** write the ioctl_cmd to execute
 * @buf: input, save the ioctl_cmd string should be execute
 */
static ssize_t hisee_ioctl_store (struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
	int ret = 0;
	int i = 0;
	int cmd_index = 0;
	char *ptr_name = NULL;

	if (NULL == buf || (strlen(buf) >= HISEE_CMD_NAME_LEN)) {
		pr_err("%s buf paramters is invalid\n", __func__);
		set_errno_and_return(HISEE_INVALID_PARAMS);
	}
	if (count <= 0 || (count >= HISEE_CMD_NAME_LEN)) {
		pr_err("%s count paramters is invalid\n", __func__);
		set_errno_and_return(HISEE_INVALID_PARAMS);
	}
	if (!g_hisee_data.rpmb_is_ready) {
		pr_err("%s rpmb is not ready now\n", __func__);
		record_hisee_log_by_dmd(DSM_NFC_HISEE_COS_IMAGE_UPGRADE_ERROR_NO, HISEE_RPMB_MODULE_INIT_ERROR);
		set_errno_and_return(HISEE_RPMB_MODULE_INIT_ERROR);
	}
	for (ptr_name = g_hisee_atf_function_list[i].function_name; ptr_name != NULL;) {
		if (!strncmp(buf, ptr_name, strlen(ptr_name))) {
			cmd_index = i;
			break;
		}
		i++;
		ptr_name = g_hisee_atf_function_list[i].function_name;
	}
	if (!ptr_name) {
		pr_err("%s cmd=%s invalid\n", __func__, buf);
		set_errno_and_return(HISEE_INVALID_PARAMS);
	}

	/* one of the three cases makes this condition is false, than do next operation:
	 * 1. cmd index is hisee_factory_action or hisee_slt_action or hisee_slt_read
	 */
	if ((cmd_index < MANAFACTOR_CMD_INDEX) && (hisee_get_power_status() != HISEE_POWER_STATUS_ON)) {
		pr_err("%s hisee is not poweron\n", __func__);
		record_hisee_log_by_dmd(DSM_NFC_HISEE_COS_IMAGE_UPGRADE_ERROR_NO, HISEE_POWERCTRL_FLOW_ERROR);
		set_errno_and_return(HISEE_POWERCTRL_FLOW_ERROR);
	}
	ret = g_hisee_atf_function_list[cmd_index].function_ptr((void *)(buf + strlen(g_hisee_atf_function_list[cmd_index].function_name)),
															(int)(long)dev);
	if (ret !=  HISEE_OK) {
		pr_err("%s ioctl_cmd:%s failed, retcode=%d\n", __func__, g_hisee_atf_function_list[cmd_index].function_name, ret);
		record_hisee_log_by_dmd(DSM_NFC_HISEE_COS_IMAGE_UPGRADE_ERROR_NO, ret);
		return HISEE_INVALID_PARAMS;
	} else {
		pr_err("%s ioctl_cmd:%s success\n", __func__, g_hisee_atf_function_list[cmd_index].function_name);
		return count;
	}
}

/** read the apdu command response,size is 0--255 bytes
 * @buf: output, save the apdu command ack data, the size is 0--255 bytes
 */
static ssize_t hisee_apdu_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	int ret = 0;

	if (NULL == buf) {
		pr_err("%s buf paramters is null\n", __func__);
		set_errno_and_return(HISEE_INVALID_PARAMS);
	}
	if (HISEE_APDU_DATA_LEN_MAX < g_hisee_data.apdu_ack.ack_len) {
		pr_err("%s BUG_ON\n", __func__);
		BUG_ON(1);/*lint !e730*/
	}
#ifdef HISEE_DEBUG
	int i;
	pr_err("%s(): ack_len=%d.\n", __func__, g_hisee_data.apdu_ack.ack_len);
	for (i = 0; i < g_hisee_data.apdu_ack.ack_len; i++)
		pr_err("[i=%3d]:%x\n", i, g_hisee_data.apdu_ack.ack_buf[i]);
#endif
	if (g_hisee_data.smc_cmd_running) {
		pr_err("%s(): apdu is running,no response\n", __func__);
		g_hisee_data.apdu_ack.ack_len = 0;
		buf[0] = 0;
	} else {
		memcpy(buf, g_hisee_data.apdu_ack.ack_buf, (u64)g_hisee_data.apdu_ack.ack_len);
		buf[g_hisee_data.apdu_ack.ack_len] = 0;
	}

	return (ssize_t)g_hisee_data.apdu_ack.ack_len;
}/*lint !e715*/

/** write the apdu command function, size is 0--261 bytes
 * @buf: input, save the  apdu command data should be execute
 */
static ssize_t hisee_apdu_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
	int ret;

	if (NULL == buf) {
		pr_err("%s buf paramters is invalid\n", __func__);
		set_errno_and_return(HISEE_INVALID_PARAMS);
	}
	if (count == 0 || (count > HISEE_APDU_DATA_LEN_MAX)) {
		pr_err("%s count paramters is invalid\n", __func__);
		set_errno_and_return(HISEE_INVALID_PARAMS);
	}
	if (!g_hisee_data.rpmb_is_ready) {
		pr_err("%s rpmb is not ready now\n", __func__);
		record_hisee_log_by_dmd(DSM_NFC_HISEE_APDU_COMMAND_OPERATION_ERROR_NO, HISEE_RPMB_MODULE_INIT_ERROR);
		set_errno_and_return(HISEE_RPMB_MODULE_INIT_ERROR);
	}

	if  (hisee_get_power_status() != HISEE_POWER_STATUS_ON) {
		pr_err("%s hisee is not poweron\n", __func__);
		record_hisee_log_by_dmd(DSM_NFC_HISEE_APDU_COMMAND_OPERATION_ERROR_NO, HISEE_POWERCTRL_FLOW_ERROR);
		set_errno_and_return(HISEE_POWERCTRL_FLOW_ERROR);
	}

#ifdef HISEE_DEBUG
	int i;
	for (i = 0; i < count; i++)
		pr_err("[%3d]:%x\n", i, buf[i]);
#endif
	ret = write_apdu_command_func((char *)buf, (unsigned int)count);
	if (ret !=  HISEE_OK) {
		record_hisee_log_by_dmd(DSM_NFC_HISEE_APDU_COMMAND_OPERATION_ERROR_NO, ret);
		return HISEE_INVALID_PARAMS;
	} else {
		pr_err("%s write_apdu_command_func success, retcode=%d\n", __func__, ret);
		return (ssize_t)count;
	}
}/*lint !e715*/

/* only root permissions can access this sysfs node */
/*lint -e84 -esym(84,*) */
/*lint -e846 -esym(846,*) */
/*lint -e514 -esym(514,*) */
/*lint -e866 -esym(866,*) */
/*lint -e778 -esym(778,*) */
static DEVICE_ATTR(hisee_ioctl, (S_IRUSR | S_IWUSR), hisee_ioctl_show, hisee_ioctl_store);
static DEVICE_ATTR(hisee_power, (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP), hisee_powerctrl_show, hisee_powerctrl_store);
static DEVICE_ATTR(hisee_apdu, (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP), hisee_apdu_show, hisee_apdu_store);
static DEVICE_ATTR(hisee_check_ready, (S_IRUSR | S_IRGRP), hisee_check_ready_show, NULL);
static DEVICE_ATTR(hisee_has_new_cos, (S_IRUSR | S_IRGRP), hisee_has_new_cos_show, NULL);
static DEVICE_ATTR(hisee_check_upgrade, (S_IRUSR | S_IRGRP), hisee_check_upgrade_show, NULL);
/*lint +e778 +esym(778,*) */
/*lint +e84 +esym(84,*) */
/*lint +e846 +esym(846,*) */
/*lint +e514 +esym(514,*) */
/*lint +e866 +esym(866,*) */

static int hisee_remove(struct platform_device *pdev)
{
	int ret = HISEE_OK;
	if (g_hisee_data.cma_device) {
		device_remove_file(g_hisee_data.cma_device, &dev_attr_hisee_ioctl);
		device_remove_file(g_hisee_data.cma_device, &dev_attr_hisee_power);
		device_remove_file(g_hisee_data.cma_device, &dev_attr_hisee_apdu);
		device_remove_file(g_hisee_data.cma_device, &dev_attr_hisee_check_ready);
		device_remove_file(g_hisee_data.cma_device, &dev_attr_hisee_has_new_cos);
		device_remove_file(g_hisee_data.cma_device, &dev_attr_hisee_check_upgrade);
		of_reserved_mem_device_release(g_hisee_data.cma_device);
	}
	return ret;
}

static int __init hisee_probe(struct platform_device *pdev)
{
	struct device *pdevice = &(pdev->dev);
	struct task_struct *rpmb_ready_task = NULL;
	unsigned int dts_u32_value = 0;
	int ret = HISEE_OK;

	memset((void *)&g_hisee_data, 0, sizeof(g_hisee_data));
	ret = of_reserved_mem_device_init(pdevice);
	if (ret != HISEE_OK) {
		pr_err("hisee shared cma pool with ATF registered failed!\n");
		set_errno_and_return(HISEE_CMA_DEVICE_INIT_ERROR);
	}
	if (of_find_property(pdevice->of_node, "hisee_partition_byname_find", NULL)) {
		g_hisee_partition_byname_find = 1;
	}

	ret = set_lpmcu_nfc_irq();
	if (HISEE_OK != ret) {
		pr_err("hisee send cmd to lpmcu failed. ignored\n");
	}

	g_hisee_data.cma_device = pdevice;
	pr_err("hisee shared cma pool with ATF registered success!\n");

	if (0 == of_property_read_u32(pdevice->of_node, "platform_id", &dts_u32_value))
		g_platform_id = dts_u32_value;

	if (0 == of_property_read_u32(pdevice->of_node, "sm_flag_pos", &dts_u32_value)) {
		g_hisee_sm_efuse_pos = dts_u32_value;
		pr_info("hisee sm_flag_pos %u\n", g_hisee_sm_efuse_pos);
	} else {
		pr_err("hisee get sm_flag_pos fail\n");
	}

	if (device_create_file(pdevice, &dev_attr_hisee_ioctl)) {
		ret = HISEE_IOCTL_NODE_CREATE_ERROR;
		pr_err("hisee err unable to create hisee_ioctl attributes\n");
		goto err_mem_release;
	}

	if (device_create_file(pdevice, &dev_attr_hisee_power)) {
		ret = HISEE_POWER_NODE_CREATE_ERROR;
		pr_err("hisee err unable to create hisee_power attributes\n");
		goto err_device_remove_file;
	}
	if (device_create_file(pdevice, &dev_attr_hisee_apdu)) {
		ret = HISEE_IOCTL_NODE_CREATE_ERROR;
		pr_err("hisee err unable to create hisee_apdu attributes\n");
		goto err_device_remove_file1;
	}

	if (device_create_file(pdevice, &dev_attr_hisee_check_ready)) {
		ret = HISEE_POWER_NODE_CREATE_ERROR;
		pr_err("hisee err unable to create hisee_check_ready attributes\n");
		goto err_device_remove_file2;
	}
	if (device_create_file(pdevice, &dev_attr_hisee_has_new_cos)) {
		ret = HISEE_POWER_NODE_CREATE_ERROR;
		pr_err("hisee err unable to create hisee_has_new_cos attributes\n");
		goto err_device_remove_file3;
	}
	if (device_create_file(pdevice, &dev_attr_hisee_check_upgrade)) {
		ret = HISEE_POWER_NODE_CREATE_ERROR;
		pr_err("hisee err unable to create hisee_check_upgrade attributes\n");
		goto err_device_remove_file4;
	}

	g_hisee_data.hisee_clk = clk_get(NULL, "hise_volt_hold");
	if (IS_ERR_OR_NULL(g_hisee_data.hisee_clk)) {
		ret = HISEE_BULK_CLK_INIT_ERROR;
		pr_err("hisee err clk_get failed\n");
		goto err_device_remove_file_end;
	}

	mutex_init(&(g_hisee_data.hisee_mutex));
	sema_init(&(g_hisee_data.atf_sem), 0);
	atomic_set(&g_hisee_errno, HISEE_OK);
	g_hisee_data.factory_test_state = HISEE_FACTORY_TEST_NORUNNING;
	hisee_power_ctrl_init();
	wake_lock_init(&g_hisee_data.wake_lock, WAKE_LOCK_SUSPEND, "hisi-hisee"); /*used in rpmb_ready_task*/

	rpmb_ready_task = kthread_run(rpmb_ready_body, NULL, "rpmb_ready_task");
	if (!rpmb_ready_task) {
		ret = HISEE_THREAD_CREATE_ERROR;
		pr_err("hisee err create rpmb_ready_task failed\n");
		goto err_device_remove_file_end;
	}

	register_flash_hisee_otp_fn(flash_hisee_debug_switchs);
	pr_err("hisee module init success!\n");
	set_errno_and_return(HISEE_OK);
err_device_remove_file_end:
	device_remove_file(pdevice, &dev_attr_hisee_check_upgrade);
err_device_remove_file4:
	device_remove_file(pdevice, &dev_attr_hisee_has_new_cos);
err_device_remove_file3:
	device_remove_file(pdevice, &dev_attr_hisee_check_ready);
err_device_remove_file2:
	device_remove_file(pdevice, &dev_attr_hisee_apdu);
err_device_remove_file1:
	device_remove_file(pdevice, &dev_attr_hisee_power);
err_device_remove_file:
	device_remove_file(pdevice, &dev_attr_hisee_ioctl);
err_mem_release:
	of_reserved_mem_device_release(g_hisee_data.cma_device);

	pr_err("hisee module init failed!\n");
	set_errno_and_return(ret);
}

static const struct of_device_id hisee_of_match[] = {
	{.compatible = "hisilicon,hisee-device" },
	{ }
};
MODULE_DEVICE_TABLE(of, hisee_of_match);

static struct platform_driver hisee_driver = {
	.probe = hisee_probe,
	.remove = hisee_remove,
	.suspend = hisee_suspend,
	.driver = {
		.name  = HISEE_DEVICE_NAME,
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(hisee_of_match),
	},
};

static int __init hisee_module_init(void)
{
	int ret = 0;
	ret = platform_driver_register(&hisee_driver);
	if (ret) {
		pr_err("register hisee driver failed.\n");
	}
	return ret;
}
static void __exit hisee_module_exit(void)
{
	platform_driver_unregister(&hisee_driver);
}
module_init(hisee_module_init);
module_exit(hisee_module_exit);

MODULE_AUTHOR("<lvtaolong@hisilicon.com>");
MODULE_DESCRIPTION("HISEE KERNEL DRIVER");
MODULE_LICENSE("GPL V2");
