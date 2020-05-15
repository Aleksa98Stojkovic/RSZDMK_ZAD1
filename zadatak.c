#include <LiquidCrystal.h>
#include <util/delay.h>

LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);

char ocitaj_taster()
{
  char taster = 0;
  unsigned char row;
  
  MCUCR &= ~0x10;
  PORTD = 0x0f; 
  
  for(row = 0x80; row != 0x08; row >>= 1)
  {
    DDRD = row;
	_delay_ms(2);
    
    switch(row | ((~PIND) & 0x0f))
    {
      case 0x88:
      	taster = '1'; break;
      case 0x84:
      	taster = '2'; break;
      case 0x82:
      	taster = '3'; break;
      case 0x81:
      	taster = 'A'; break;
		
	  case 0x48:
      	taster = '4'; break;
      case 0x44:
      	taster = '5'; break;
      case 0x42:
      	taster = '6'; break;
      case 0x41:
      	taster = 'B'; break;
		
	  case 0x28:
      	taster = '7'; break;
      case 0x24:
      	taster = '8'; break;
      case 0x22:
      	taster = '9'; break;
      case 0x21:
      	taster = 'C'; break;
		
	  case 0x18:
      	taster = '*'; break;
      case 0x14:
      	taster = '0'; break;
      case 0x12:
      	taster = '#'; break;
      case 0x11:
      	taster = 'D'; break;
    }
  }
  
  return taster;
}

byte herc [8] = {
	B00000,
	B01010,
	B11111,
	B11111,
	B01110,
	B00100,
	B00000,
};

byte karo [8] = {
	B00000,
	B00100,
	B01110,
	B11111,
	B01110,
	B00100,
	B00000,
};

byte pik [8] = {
	B00000,
	B00100,
	B01110,
	B11111,
	B11111,
	B01010,
	B00000,
};

byte tref [8] = {
	B00000,
	B00100,
	B01110,
	B00100,
	B11111,
	B01110,
	B00100,
};

bool proveri(int *oblici, int *oblici2)
{
	for(int i = 0; i < 4; i++)
	{
		if(oblici2[i] != oblici[i])
			return false;
	}
	
	return true;
}

// ovu funkciju koristim kako
void inicijalizuj(int *oblici, int *lut)
{
	int brojac = 0, pom;
	do
	{
		pom = random(4);
		if(lut[pom] == 0)
		{	
			oblici[brojac] = pom;
			lut[pom] = 1;
			brojac++;
		}
		
	}while(brojac != 4);
}

int main()
{
  	lcd.begin(16, 2);
	
	lcd.createChar(0, herc);
	lcd.createChar(1, karo);
	lcd.createChar(2, pik);
	lcd.createChar(3, tref);
	
	//randomSeed(analogRead(0)); // ne radi sa randomSeed
	
	char simbol;
	char psimbol = 0;
	
	int oblici[4], lut[4] = {0, 0, 0, 0};
	int oblici2[4];
	
	int karakter = 0, col = 0;
	
	while(1)
    {
		simbol = ocitaj_taster();
		
		if(simbol != psimbol)
		{
			psimbol = simbol;
			
			if(simbol == '*')
			{
				lcd.clear();
				
				inicijalizuj(oblici, lut);
				
				for(int i = 0; i < 4; i++)
				{
					lcd.setCursor(i, 0);
					lcd.write(byte(oblici[i]));
				}
				
				_delay_ms(2000);
				
				lcd.clear();
				
				for(int i = 0; i < 4; i++)
				{
					lcd.print('-');
				}
				
				lcd.setCursor(0, 0);
				lcd.blink();
			}
			else
			{
				if(simbol != '5')
				{
					switch(simbol)
					{
						case '2': 
							if(karakter != 3)
							{
								karakter++;	
								oblici2[col] = karakter;
								lcd.setCursor(col, 0);
								lcd.blink();
								lcd.write(byte(karakter));
							}
							break;
							
						case '8':
							if(karakter != 0)
							{
								karakter--;
								oblici2[col] = karakter;
								lcd.setCursor(col, 0);
								lcd.blink();
								lcd.write(byte(karakter));
							}
							break;
							
						case '4':
							if(col != 0)
								col--;
							break;
							
						case '6':
							if(col != 3)
								col++;
							break;
					}
				}
				else
				{
					lcd.noBlink();
					
					//ispis starih karaktera
					for(int i = 0; i < 4; i++)
					{
						lcd.setCursor(i, 1);
						lcd.write(byte(oblici[i]));
						
						lut[i] = 0;
					}
					
					if(proveri(oblici, oblici2))
					{
						lcd.setCursor(8, 1);
						lcd.print("Tacno");
					}
					else
					{
						lcd.setCursor(8, 1);
						lcd.print("Netacno");
					}
				}
			}
			
		}
	}
  
  	return 0;
}