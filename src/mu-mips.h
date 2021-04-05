#include <stdint.h>

#define FALSE 0
#define TRUE  1

/******************************************************************************/
/* MIPS memory layout                                                                                                                                      */
/******************************************************************************/
#define MEM_TEXT_BEGIN  0x00400000
#define MEM_TEXT_END      0x0FFFFFFF
/*Memory address 0x10000000 to 0x1000FFFF access by $gp*/
#define MEM_DATA_BEGIN  0x10010000
#define MEM_DATA_END   0x7FFFFFFF

#define MEM_KTEXT_BEGIN 0x80000000
#define MEM_KTEXT_END  0x8FFFFFFF

#define MEM_KDATA_BEGIN 0x90000000
#define MEM_KDATA_END  0xFFFEFFFF

/*stack and data segments occupy the same memory space. Stack grows backward (from higher address to lower address) */
#define MEM_STACK_BEGIN 0x7FFFFFFF
#define MEM_STACK_END  0x10010000

typedef struct {
	uint32_t begin, end;
	uint8_t *mem;
} mem_region_t;

/* memory will be dynamically allocated at initialization */
mem_region_t MEM_REGIONS[] = {
	{ MEM_TEXT_BEGIN, MEM_TEXT_END, NULL },
	{ MEM_DATA_BEGIN, MEM_DATA_END, NULL },
	{ MEM_KDATA_BEGIN, MEM_KDATA_END, NULL },
	{ MEM_KTEXT_BEGIN, MEM_KTEXT_END, NULL }
};

#define NUM_MEM_REGION 4
#define MIPS_REGS 32

typedef struct CPU_State_Struct {

  uint32_t PC;		                   /* program counter */
  uint32_t REGS[MIPS_REGS]; /* register file. */
  uint32_t HI, LO;                          /* special regs for mult/div. */
} CPU_State;

typedef struct CPU_Pipeline_Reg_Struct{
	uint32_t PC;
	uint32_t IR;
	uint32_t A;
	uint32_t B;
	uint32_t sham_t;
	uint32_t imm;
	uint32_t ALUOutput;
	uint32_t ALUOutputLow;
	uint32_t LMD;
	int stage_stalled; //1 for bubble, 0 for standard
	int Forwarding_Type;
	int MEM_ACCESS_FLAG; //is there a memory access in this command?
	int REG_WRITE_FLAG; //is there writing back to a reg during this command that could cause a data hazard?
	uint32_t REG_RD_VALUE; //for checking for data hazards
	uint32_t REG_RS_VALUE;
	uint32_t REG_RT_VALUE;

} CPU_Pipeline_Reg;

/***************************************************************/
/* CPU State info.                                                                                                               */
/***************************************************************/

CPU_State CURRENT_STATE, NEXT_STATE;
int RUN_FLAG;	/* run flag*/
uint32_t INSTRUCTION_COUNT=-3;
uint32_t CYCLE_COUNT;
uint32_t PROGRAM_SIZE; /*in words*/


/***************************************************************/
/* Pipeline Registers.                                                                                                        */
/***************************************************************/
CPU_Pipeline_Reg IF_ID;
CPU_Pipeline_Reg ID_EX;
CPU_Pipeline_Reg EX_MEM;
CPU_Pipeline_Reg MEM_WB;

char prog_file[32];

int ENABLE_FORWARDING = 1; //forwarding enable flag
int STALL_COUNT = 0; //flag for stalling
int FORWARDING_TYPE; //one for reg to alu output, two for 


/***************************************************************/
/* Function Declerations.                                                                                                */
/***************************************************************/
void help();
uint32_t mem_read_32(uint32_t address);
void mem_write_32(uint32_t address, uint32_t value);
void cycle();
void run(int num_cycles);
void runAll();
void mdump(uint32_t start, uint32_t stop) ;
void rdump();
void handle_command();
void reset();
void init_memory();
void load_program();
void handle_pipeline(); /*IMPLEMENT THIS*/
void WB();/*IMPLEMENT THIS*/
void MEM();/*IMPLEMENT THIS*/
void EX();/*IMPLEMENT THIS*/
void ID();/*IMPLEMENT THIS*/
void IF();/*IMPLEMENT THIS*/
void show_pipeline();/*IMPLEMENT THIS*/
void initialize();
void print_program(); /*IMPLEMENT THIS*/
void print_instruction(uint32_t address);
uint32_t ALUOperationI();
uint32_t ALUOperationR();
int load_store(uint32_t opcode, uint32_t regreg);
int reg_imm(uint32_t opcode);
int reg_reg(uint32_t opcode, uint32_t instruction);
void rtypeWB(uint32_t funct, uint32_t rd);
unsigned createMask(int start, int end);
unsigned applyMask(unsigned mask, uint instruction);
void dataHazardDetection();