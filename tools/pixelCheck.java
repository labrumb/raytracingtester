import java.awt.image.BufferedImage;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;

import javax.imageio.ImageIO;

/*
 * takes in an image location pixel coordinates and rgb values and checks that pixel is that color
 */
public class pixelCheck {

	public static void main(String[] args) {
		if(args.length!=4)
		{
			System.out.println("invalid input");
			return;
		}
		String location;
		int x;
		int y;
		int size;
		
		
		//get information from input
		try{
			location = args[0];
			
			if(!location.substring(location.length()-1).equals("/"))
				location = location+"/";
			
			x = Integer.parseInt(args[1]);
			y = Integer.parseInt(args[2]);
			size = Integer.parseInt(args[3]);
		}catch(Exception e){
			System.out.println("invalid input");
			return;
		}
		
		//loop through directory
		File[] files = new File(location).listFiles();
		//If this pathname does not denote a directory, then listFiles() returns null. 

		for (File file : files) {
		    if (file.isFile() && file.getName().substring(file.getName().length()-4).equals(".jpg")) {
			System.out.print(file.getName());
		        test(x,y,size,location+file.getName());
		    }
		}
		

	}
	
	static void test(int x,int y,int size, String location)
	{
		try {
			File image = new File(location);
			BufferedImage buf;
			FileInputStream fis = new FileInputStream(image);
			buf = ImageIO.read(fis);
			//get color of pixel
			int clr = buf.getRGB(x,y);
			int  green = (clr & 0x0000ff00) >> 8;
			int  red   = (clr & 0x00ff0000) >> 16;
			int  blue  =  clr & 0x000000ff;
		
			//check pixel location
			if(green>0 && red == 0 && blue == 0)
				System.out.print(" pass ");
			else
				System.out.print(" failed ");
			//check all pixels			
			for(int i = 0; i < size; i++)
				for(int j = 0; j<size; j++)
				{
					  clr = buf.getRGB(i,j); 
					  red   = (clr & 0x00ff0000) >> 16;
					  green = (clr & 0x0000ff00) >> 8;
					  blue  =  clr & 0x000000ff;
					  if(green>0 && red == 0 && blue == 0)
					  {
						  System.out.print("pass \n");
						  return;
					  }
				}
			System.out.print("failed \n");
			return;
		
		} catch (IOException e) {
			System.out.println("invalid image location");
			return;
		}
		
	}
	
}
