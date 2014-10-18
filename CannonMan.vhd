LIBRARY ieee;

USE ieee.std_logic_1164.all; 

USE ieee.std_logic_arith.all; 

USE ieee.std_logic_unsigned.all; 

ENTITY CannonMan IS

   PORT (
		-- SYSTEM & PERIPHERALS
      SW 		: IN STD_LOGIC_VECTOR(17 DOWNTO 0);
      KEY 		: IN STD_LOGIC_VECTOR(3 DOWNTO 0);
		LEDG 		: OUT STD_LOGIC_VECTOR(7 downto 0);
		LEDR		: OUT STD_LOGIC_VECTOR(17 downto 0);
      CLOCK_50 : IN STD_LOGIC;
		CLOCK_27 : IN	STD_LOGIC;
		AUD_XCK	: OUT	STD_LOGIC;
		
		HEX0		: OUT STD_LOGIC_VECTOR(6 DOWNTO 0);
		HEX1		: OUT STD_LOGIC_VECTOR(6 DOWNTO 0);
		HEX2		: OUT STD_LOGIC_VECTOR(6 DOWNTO 0);
		HEX3		: OUT STD_LOGIC_VECTOR(6 DOWNTO 0);
		HEX4		: OUT STD_LOGIC_VECTOR(6 DOWNTO 0);
		HEX5		: OUT STD_LOGIC_VECTOR(6 DOWNTO 0);
		HEX6		: OUT STD_LOGIC_VECTOR(6 DOWNTO 0);
		HEX7		: OUT STD_LOGIC_VECTOR(6 DOWNTO 0);
		
		-- SDRAM
      DRAM_CLK, DRAM_CKE 		: OUT 	STD_LOGIC;
      DRAM_ADDR 					: OUT 	STD_LOGIC_VECTOR(11 DOWNTO 0);
      DRAM_BA_0, DRAM_BA_1 	: BUFFER STD_LOGIC;
      DRAM_CS_N, DRAM_CAS_N	: BUFFER STD_LOGIC;
		DRAM_RAS_N, DRAM_WE_N 	: OUT 	STD_LOGIC;
      DRAM_DQ 						: INOUT 	STD_LOGIC_VECTOR(15 DOWNTO 0);
      DRAM_UDQM, DRAM_LDQM 	: BUFFER STD_LOGIC;
		
		-- LCD
		LCD_BLON, LCD_EN, LCD_ON, LCD_RS, LCD_RW : OUT	STD_LOGIC;
		LCD_DATA						: INOUT	STD_LOGIC_VECTOR(7  DOWNTO 0);
		
		-- SD CARD
		SD_CLK						: OUT		STD_LOGIC;
		SD_CMD						: INOUT 	STD_LOGIC;
		SD_DAT						: INOUT	STD_LOGIC;
		SD_DAT3						: INOUT	STD_LOGIC;
		
		-- VGA
		VGA_BLANK					: OUT		STD_LOGIC;
		VGA_B							: OUT		STD_LOGIC_VECTOR(9 DOWNTO 0);
		VGA_CLK						: OUT		STD_LOGIC;
		VGA_G							: OUT		STD_LOGIC_VECTOR(9 DOWNTO 0);
		VGA_HS						: OUT 	STD_LOGIC;
		VGA_R							: OUT		STD_LOGIC_VECTOR(9 DOWNTO 0);
		VGA_SYNC						: OUT		STD_LOGIC;
		VGA_VS						: OUT 	STD_LOGIC;
		
		-- SRAM
		SRAM_DQ		:	INOUT	STD_LOGIC_VECTOR(15	downto	0);
		SRAM_ADDR	:	OUT	STD_LOGIC_VECTOR(17	downto	0);
		SRAM_LB_N	:	OUT	STD_LOGIC;
		SRAM_UB_N	:	OUT	STD_LOGIC;
		SRAM_CE_N	:	OUT	STD_LOGIC;
		SRAM_OE_N	:	OUT	STD_LOGIC;
		SRAM_WE_N	:	OUT	STD_LOGIC;
		
		-- PS/2
		PS2_CLK						: INOUT	STD_LOGIC;
		PS2_DAT						: INOUT 	STD_LOGIC;
		
		-- AUDIO
		AUD_BCLK		: 	IN 	STD_LOGIC; 
		AUD_DACDAT	:	OUT	STD_LOGIC;
		AUD_DACLRCK	:	IN		STD_LOGIC;
		AUD_ADCDAT	:	IN		STD_LOGIC;
		AUD_ADCLRCK	:	IN		STD_LOGIC;
		
		-- A/V CONFIG
		I2C_SDAT		:	INOUT	STD_LOGIC;
		I2C_SCLK		:	OUT	STD_LOGIC
		);
		
END CannonMan;

ARCHITECTURE Structure OF CannonMan IS

   COMPONENT nios2_system PORT (
		-- system
      clk_clk 			: IN STD_LOGIC;
      reset_reset_n 	: IN STD_LOGIC;
		audio_clk_clk	:	out STD_LOGIC;
		audio_secondary_clk: in STD_LOGIC;
		
		-- lcd_data
		lcd_data_DATA 	: INOUT 	STD_LOGIC_VECTOR(7 downto 0);
		lcd_data_ON		: OUT		STD_LOGIC;
		lcd_data_BLON	: OUT		STD_LOGIC;
		lcd_data_EN		: OUT		STD_LOGIC;
		lcd_data_RS		: OUT		STD_LOGIC;
		lcd_data_RW		: OUT		STD_LOGIC;
		
		-- keys
		keys_export		: IN		STD_LOGIC_VECTOR(3 downto 0);
		
		-- switches
		switches_export	: IN		STD_LOGIC_VECTOR(17 downto 0);
		
		-- LEDs
		ledg_export		: OUT		STD_LOGIC_VECTOR(7 downto 0);
		ledr_export		: OUT		STD_LOGIC_VECTOR(17 downto 0);
		
		-- sdram
      sdram_wire_addr 	: OUT 	STD_LOGIC_VECTOR(11 DOWNTO 0);
      sdram_wire_ba 		: BUFFER STD_LOGIC_VECTOR(1 DOWNTO 0);
      sdram_wire_cas_n 	: OUT 	STD_LOGIC;
      sdram_wire_cke 	: OUT 	STD_LOGIC;
      sdram_wire_cs_n 	: OUT 	STD_LOGIC;
      sdram_wire_dq 		: INOUT 	STD_LOGIC_VECTOR(15 DOWNTO 0);
      sdram_wire_dqm 	: BUFFER STD_LOGIC_VECTOR(1 DOWNTO 0);
      sdram_wire_ras_n 	: OUT 	STD_LOGIC;
      sdram_wire_we_n 	: OUT 	STD_LOGIC;
      sdram_clk_clk 		: OUT 	STD_LOGIC;
		
		-- VGA
		video_vga_controller_0_external_interface_CLK 	: OUT STD_LOGIC;
		video_vga_controller_0_external_interface_HS  	: OUT STD_LOGIC;
		video_vga_controller_0_external_interface_VS		: OUT STD_LOGIC;
		video_vga_controller_0_external_interface_BLANK : OUT STD_LOGIC;
		video_vga_controller_0_external_interface_SYNC	: OUT STD_LOGIC;
		video_vga_controller_0_external_interface_R		: OUT STD_LOGIC_VECTOR(9 downto 0);
		video_vga_controller_0_external_interface_G		: OUT STD_LOGIC_VECTOR(9 downto 0);
		video_vga_controller_0_external_interface_B		: OUT STD_LOGIC_VECTOR(9 downto 0);
		
		-- SD Card
		sdcard_b_SD_cmd	: INOUT STD_LOGIC;
		sdcard_b_SD_dat	: INOUT STD_LOGIC;
		sdcard_b_SD_dat3	: INOUT STD_LOGIC;
		sdcard_o_SD_clock : OUT   STD_LOGIC;
		
		-- SRAM
		sram_0_external_interface_DQ                    : inout std_logic_vector(15 downto 0) := (others => 'X'); -- DQ
		sram_0_external_interface_ADDR                  : out   std_logic_vector(17 downto 0);                    -- ADDR
		sram_0_external_interface_LB_N                  : out   std_logic;                                        -- LB_N
		sram_0_external_interface_UB_N                  : out   std_logic;                                        -- UB_N
		sram_0_external_interface_CE_N                  : out   std_logic;                                        -- CE_N
		sram_0_external_interface_OE_N                  : out   std_logic;                                        -- OE_N
		sram_0_external_interface_WE_N                  : out   std_logic;
		
		-- PS2
		ps2_peri_CLK				: INOUT STD_LOGIC;
		ps2_peri_DAT				: INOUT STD_LOGIC;
		
		-- AUDIO
		audio_BCLK                 : in    std_logic;
		audio_DACDAT               : out   std_logic;
		audio_DACLRCK              : in    std_logic;
		audio_ADCDAT					: in	  std_logic;
		audio_ADCLRCK					: in 	  std_logic;
		
		-- A/V CONFIG
		av_config_SDAT				: inout	std_LOGIC;
		av_config_sclk				: out		std_logic
		);
   END COMPONENT;

   SIGNAL DQM : STD_LOGIC_VECTOR(1 DOWNTO 0);
   SIGNAL BA : STD_LOGIC_VECTOR(1 DOWNTO 0);

   BEGIN
      DRAM_BA_0 <= BA(0);
      DRAM_BA_1 <= BA(1);
      DRAM_UDQM <= DQM(1);
      DRAM_LDQM <= DQM(0);
		
		-- turn that sh!t off for eye protection
		-- Lomash: Safety first.
		HEX0 <= "1111111";
		HEX1 <= "1111111";
		HEX2 <= "1111111";
		HEX3 <= "1111111";
		HEX4 <= "1111111";
		HEX5 <= "1111111";
		HEX6 <= "1111111";
		HEX7 <= "1111111";
		
      NiosII: nios2_system
			PORT MAP (
				clk_clk => CLOCK_50,				
				reset_reset_n => SW(17),
				audio_clk_clk => AUD_XCK,
				audio_secondary_clk => CLOCK_27,
				
				keys_export => KEY,
				ledg_export => LEDG,
				ledr_export => LEDR,
				switches_export => SW,
				
				lcd_data_DATA => LCD_DATA,
				lcd_data_ON => LCD_ON,
				lcd_data_BLON => LCD_BLON,
				lcd_data_EN	=> LCD_EN,
				lcd_data_RS	=> LCD_RS,
				lcd_data_RW	=> LCD_RW,
				
				sdram_clk_clk => DRAM_CLK,
				sdram_wire_addr => DRAM_ADDR,
				sdram_wire_ba => BA,
				sdram_wire_cas_n => DRAM_CAS_N,
				sdram_wire_cke => DRAM_CKE,
				sdram_wire_cs_n => DRAM_CS_N,
				sdram_wire_dq => DRAM_DQ,
				sdram_wire_dqm => DQM,
				sdram_wire_ras_n => DRAM_RAS_N,
				sdram_wire_we_n => DRAM_WE_N,
				
				video_vga_controller_0_external_interface_CLK	=>	VGA_CLK,	
				video_vga_controller_0_external_interface_HS	=>	VGA_HS,	
				video_vga_controller_0_external_interface_VS	=>	VGA_VS,	
				video_vga_controller_0_external_interface_BLANK	=>	VGA_BLANK,	
				video_vga_controller_0_external_interface_SYNC	=>	VGA_SYNC,	
				video_vga_controller_0_external_interface_R	=>	VGA_R,	
				video_vga_controller_0_external_interface_G	=>	VGA_G,	
				video_vga_controller_0_external_interface_B	=>	VGA_B,
				
				sram_0_external_interface_DQ		=>	SRAM_DQ,
				sram_0_external_interface_ADDR	=>	SRAM_ADDR,
				sram_0_external_interface_LB_N	=>	SRAM_LB_N,
				sram_0_external_interface_UB_N	=>	SRAM_UB_N,			
				sram_0_external_interface_CE_N	=>	SRAM_CE_N,
				sram_0_external_interface_OE_N	=>	SRAM_OE_N,
				sram_0_external_interface_WE_N	=>	SRAM_WE_N,
				
				sdcard_b_SD_cmd  	=>	SD_CMD,
            sdcard_b_SD_dat  	=> SD_DAT,
            sdcard_b_SD_dat3 	=>	SD_DAT3,
            sdcard_o_SD_clock	=> SD_CLK,
				
				ps2_peri_CLK => PS2_CLK,
            ps2_peri_DAT => PS2_DAT,
				
				av_config_SDAT => I2C_SDAT,
				av_config_SCLK => I2C_SCLK,
				
				audio_BCLK		=>  AUD_BCLK,
				audio_DACDAT   =>  AUD_DACDAT,
				audio_DACLRCK  =>  AUD_DACLRCK,
				audio_ADCDAT	=>  AUD_ADCDAT,
				audio_ADCLRCK	=>	 AUD_ADCLRCK
			);
   END Structure;