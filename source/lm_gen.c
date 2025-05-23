#include <stdio.h>
#include <string.h>
#include "lm_gen.h"
#include "lm_log.h"
#include "lm_error.h"
#include "lm_parser.h"
#include "lm_string.h"



int lm_gen_header_file(const char* file_path)
{
    lm_macro_head_t* macro_list = lm_parser_get_macro_head();
    lm_list_node_t *node = lm_list_next_node(&macro_list->node);
    lm_macro_t *macro = NULL;

    FILE* file = fopen(file_path, "w");
    if (file == NULL) {
        printf("Failed to open or create the %s\n", file_path);
        return LM_ERR;
    }

    fprintf(file, "//****************************************************************\n");
    fprintf(file, "//* lite-manager                                                 *\n");
    fprintf(file, "//* NOTE: do not edit this file as it is automatically generated *\n");
    fprintf(file, "//****************************************************************\n\n");
    fprintf(file, "#ifndef  __CONFIG_H__\n#define  __CONFIG_H__\n\n\n");

    for(int i = 0; i < macro_list->count; i++) {

        macro = container_of(node, lm_macro_t, node);
        if(macro == NULL) {
            return LM_ERR;
        }

        if(macro->value != NULL) {

            if(strcmp(macro->value, " ") == 0){
                fprintf(file, "// %s is not set\n", macro->name);
            }

            if(strcmp(macro->value, "y") == 0) {
                if (fprintf(file, "#define    %-30s     1\n", macro->name) < 0) {
                    printf("Failed to write to the %s\n", file_path);
                    return LM_ERR;
                }
            }
            else if(strcmp(macro->value, "'n'") == 0) {
                if (fprintf(file, "#define    %-30s     n\n", macro->name) < 0) {
                    printf("Failed to write to the %s\n", file_path);
                    return LM_ERR;
                }
            }
            else if(strcmp(macro->value, " ") != 0 && strcmp(macro->value, "n") != 0){
                if (fprintf(file, "#define    %-30s     %-s\n", macro->name, macro->value) < 0) {
                    printf("Failed to write to the %s\n", file_path);
                    return LM_ERR;
                }
            }
        }

        node = lm_list_next_node(node);
    }

    fprintf(file, "\n\n#endif  //!__CONFIG_H__\n");
    
    fclose(file);
    return LM_OK;
}


int lm_gen_lmmk_file(const char* file_path)
{
    lm_macro_head_t* macro_list = lm_parser_get_macro_head();
    lm_list_node_t *node = lm_list_next_node(&macro_list->node);
    lm_macro_t *macro = NULL;

    FILE* file = fopen(file_path, "w");
    if (file == NULL) {
        printf("Failed to open or create the %s\n", file_path);
        return LM_ERR;
    }

    fprintf(file, "#****************************************************************\n");
    fprintf(file, "#* lite-manager                                                 *\n");
    fprintf(file, "#* NOTE: do not edit this file as it is automatically generated *\n");
    fprintf(file, "#****************************************************************\n\n");

    for(int i = 0; i < macro_list->count; i++)  {

        macro = container_of(node, lm_macro_t, node);
        if(macro == NULL) {
            return LM_ERR;
        }

        if(macro->value != NULL) {
            if (fprintf(file, "%s = %s\n", macro->name, macro->value) < 0) {
                printf("Failed to write to the %s\n", file_path);
                return LM_ERR;
            }
        }

        node = lm_list_next_node(node);
    }

    fprintf(file, "\n# Variables provided for Makefile\n");

    int len = lm_parser_get_parser_list_count() / sizeof(lm_array_t);
    lm_array_t *list = (lm_array_t*)lm_parser_get_parser_list_head();
    char *name = NULL;

    for(int i = 0; i < len; i++) {

        if(list->count != 0) {
            name = lm_parser_get_parser_list_name(i);
            if(name != NULL) {
                fprintf(file, "%s := ", name);
            }

            lm_array_print_with_max_len(file, list, 15);
            fprintf(file, "\n");
        }

        list ++;
    }

    fclose(file);
    return LM_OK;
}



int lm_gen_projcfg_file(const char* file_path)
{
    if(access(file_path, F_OK) == 0) {
        return LM_OK;
    }

    FILE* file = fopen(file_path, "w");
    if (file == NULL) {
        LM_LOG_ERROR("Failed to open or create the %s\n", file_path);
        return LM_ERR;
    }

    fprintf(file, "#****************************************************************\n");
    fprintf(file, "#* lite-manager                                                 *\n");
    fprintf(file, "#* NOTE: You can edit this file to configure the project        *\n");
    fprintf(file, "#****************************************************************\n\n");

    fclose(file);
    return LM_OK;
}


int lm_gen_mkfile_file(const char *makefile, const char *lmmk, const char *lmcfg, const char *projcfg, 
                              const char *header_file, const char *pro_name, const char *build_dir, const char *gcc_prefix)
{
    FILE* file = fopen(makefile, "w");
    if (file == NULL) {
        LM_LOG_ERROR("Failed to generate the %s file\n", makefile);
        return LM_ERR;
    }

    const char *target = NULL;

    if(lm_parser_lds_is_empty()) {
        target = ".exe";
    }
    else {
        target = ".elf";
    }

    fprintf(file, "TARGET    := %s\n", pro_name);
    fprintf(file, "BUILD_DIR := %s\n", build_dir);
    fprintf(file, "\n\n");

    fprintf(file, "# include configuration file for makefile\n");
    fprintf(file, ".PHONY: check_lmmk\n");
    fprintf(file, "ifneq ($(wildcard %s),)\n", lmmk);
    fprintf(file, "-include %s\n", lmmk);
    fprintf(file, "else\n");
    fprintf(file, "check_lmmk:\n");
    fprintf(file, "\t@echo \"Please run 'make config'\"\n");
    fprintf(file, "endif\n");
    fprintf(file, "\n\n");

    fprintf(file, "# toolchain\n");
    fprintf(file, "CC_PREFIX ?= %s\n", gcc_prefix);
    fprintf(file, "CC = $(CC_PREFIX)gcc\n");
    fprintf(file, "AS = $(CC_PREFIX)gcc -x assembler-with-cpp\n");
    fprintf(file, "CP = $(CC_PREFIX)objcopy\n");
    fprintf(file, "SZ = $(CC_PREFIX)size\n");
    fprintf(file, "OD = $(CC_PREFIX)objdump\n");
    fprintf(file, "HEX = $(CP) -O ihex\n");
    fprintf(file, "BIN = $(CP) -O binary -S\n");
    fprintf(file, "\n\n");


    fprintf(file, "CFLAGS    := $(%s) $(%s) $(%s) $(%s) $(%s)\n", VAR_MC_FLAG, VAR_C_PATH, VAR_C_DEFINE, VAR_C_FLAG, VAR_CPP_FLAG);
    fprintf(file, "ASFLAGS   := $(%s) $(%s) $(%s) $(%s) \n", VAR_MC_FLAG, VAR_C_PATH, VAR_C_DEFINE, VAR_AS_FLAG);
    
    if(lm_parser_lds_is_empty()) {
        fprintf(file, "LDFLAGS   := $(%s) $(%s) $(%s) $(%s) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map\n", VAR_MC_FLAG, VAR_LD_FLAG, VAR_LIB_PATH, VAR_LIB_NAME);
    }
    else {
        fprintf(file, "LDFLAGS   := $(%s) $(%s) $(%s) $(%s) -T$(%s) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map\n", VAR_MC_FLAG, VAR_LD_FLAG, VAR_LIB_PATH, VAR_LIB_NAME, VAR_LDS_SOURCE);
    }
    fprintf(file, "\n\n");

    fprintf(file, ".PHONY: all\n");
    if(lm_parser_lds_is_empty()) {
        fprintf(file, "all: $(BUILD_DIR)/$(TARGET)%s elf_info\n", target);
    }
    else {
        fprintf(file, "all: $(BUILD_DIR)/$(TARGET)%s $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin elf_info\n", target);
    }
    fprintf(file, "\n\n");

    fprintf(file, "# list of c and c++ program objects\n");
    fprintf(file, "OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(patsubst %%.c, %%.o, $(patsubst %%.cpp, %%.o, $(%s)))))\n", VAR_C_SOURCE);
    fprintf(file, "vpath %%.c $(sort $(dir $(%s)))\n", VAR_C_SOURCE);
    fprintf(file, "vpath %%.cpp $(sort $(dir $(%s)))\n", VAR_C_SOURCE);
    fprintf(file, "# list of ASM program objects\n");
    fprintf(file, "OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(%s:.S=.o)))\n", VAR_ASM_SOURCE);
    fprintf(file, "vpath %%.S $(sort $(dir $(%s)))\n", VAR_ASM_SOURCE);
    fprintf(file, "\n\n");


    fprintf(file, "$(BUILD_DIR)/%%.o: %%.c Makefile | $(BUILD_DIR)\n");
    fprintf(file, "\t@echo \"CC   $<\"\n");
    fprintf(file, "\t@$(CC) -c $(CFLAGS) -MMD -MP \\\n");
    fprintf(file, "\t\t-MF  $(BUILD_DIR)/$(notdir $(<:.c=.d)) \\\n");
    fprintf(file, "\t\t-Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@\n");
    fprintf(file, "\n");

    fprintf(file, "$(BUILD_DIR)/%%.o: %%.cpp Makefile | $(BUILD_DIR)\n");
    fprintf(file, "\t@echo \"CC   $<\"\n");
    fprintf(file, "\t@$(CC) -c $(CFLAGS) -MMD -MP \\\n");
    fprintf(file, "\t\t-MF  $(BUILD_DIR)/$(notdir $(<:.cpp=.d)) \\\n");
    fprintf(file, "\t\t-Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.cpp=.lst)) $< -o $@\n");
    fprintf(file, "\n");

    fprintf(file, "$(BUILD_DIR)/%%.o: %%.S Makefile | $(BUILD_DIR)\n");
    fprintf(file, "\t@echo \"AS   $<\"\n");
    fprintf(file, "\t@$(AS) -c $(ASFLAGS) -MMD -MP  \\\n");
    fprintf(file, "\t\t-MF $(BUILD_DIR)/$(notdir $(<:.S=.d)) $< -o $@\n");
    fprintf(file, "\n\n");

    fprintf(file, "$(BUILD_DIR)/$(TARGET)%s: $(OBJECTS) Makefile\n", target);
    fprintf(file, "\t@echo \"LD   $@\"\n");
    fprintf(file, "\t@$(CC) $(OBJECTS) $(LDFLAGS) -o $@\n");
    fprintf(file, "\t@$(OD) $(BUILD_DIR)/$(TARGET)%s -xS > $(BUILD_DIR)/$(TARGET).s $@\n", target);
    fprintf(file, "\t@echo ''\n");
    fprintf(file, "\t@echo \"Build Successful!\"\n");
    fprintf(file, "\t@echo \"ELF   $@\"\n");
    fprintf(file, "\n\n");


    if(!lm_parser_lds_is_empty()) {
        fprintf(file, "$(BUILD_DIR)/%%.hex: $(BUILD_DIR)/%%.elf | $(BUILD_DIR)\n");
        fprintf(file, "\t@echo \"HEX   $@\"\n");
        fprintf(file, "\t@$(HEX) $< $@\n");
        fprintf(file, "\n");

        fprintf(file, "$(BUILD_DIR)/%%.bin: $(BUILD_DIR)/%%.elf | $(BUILD_DIR)\n");
        fprintf(file, "\t@echo \"BIN   $@\"\n");
        fprintf(file, "\t@$(BIN) $< $@\n");
        fprintf(file, "\n");
    }

    fprintf(file, "elf_info: $(BUILD_DIR)/$(TARGET)%s\n", target);
    fprintf(file, "\t@echo \"==================================================================\"\n");
    fprintf(file, "\t@$(SZ) $<\n");
    fprintf(file, "\t@echo \"==================================================================\"\n");
    fprintf(file, "\n\n");

    fprintf(file, "$(BUILD_DIR):\n");
    fprintf(file, "\t@mkdir $@\n");
    fprintf(file, "\n\n");

    fprintf(file, "# Pseudo command\n");
    fprintf(file, ".PHONY: config clean\n");
    fprintf(file, "\n");

    fprintf(file, "# Check if the %s file exists\n", projcfg);
    fprintf(file, "config: %s\n", projcfg);
    fprintf(file, "\t@./lm.exe --projcfg %s --lmcfg %s --out %s --mem 50\n", projcfg, lmcfg, header_file);
    fprintf(file, "\t@./lm.exe --rmdir $(BUILD_DIR)\n");
    fprintf(file, "\n\n");

    fprintf(file, "# clean command, delete build directory\n");
    fprintf(file, "clean:\n");
    fprintf(file, "\t@./lm.exe --rmdir $(BUILD_DIR)\n");
    fprintf(file, "\n");

    fclose(file);
    return LM_OK;
}

