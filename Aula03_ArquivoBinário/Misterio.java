import java.io.*;

public class Misterio
{

	public static void escreveIntInvertido(DataOutputStream dout, int x) throws IOException
	{
		int b4 = ( x & 0xff000000  ) >> 24;
		int b3 = ( x & 0x00ff0000  ) >> 16;
		int b2 = ( x & 0x0000ff00  ) >> 8;
		int b1 = ( x & 0x000000ff  );

		dout.write(b1);
		dout.write(b2);
		dout.write(b3);
		dout.write(b4);
	}

	public static void escreveInt2Invertido(DataOutputStream dout, int x) throws IOException
	{
		int b2 = ( x & 0x0000ff00  ) >> 8;
		int b1 = ( x & 0x000000ff  );

		dout.write(b1);
		dout.write(b2);
	}

	public static void main(String args[])
	{

		OutputStream saida = null;
		DataOutputStream dout = null;

		try
		{
			saida = new FileOutputStream("teste.bmp");
			dout = new DataOutputStream(saida);

			dout.write((byte)'B');
			dout.write((byte)'M');

			escreveIntInvertido(dout,800*600*3+54);
			dout.writeInt(0);
			escreveIntInvertido(dout,54);

			escreveIntInvertido(dout,40);
			escreveIntInvertido(dout,800);
			escreveIntInvertido(dout,600);
			escreveInt2Invertido(dout,1);
			escreveInt2Invertido(dout,24);
			escreveIntInvertido(dout,0);
			escreveIntInvertido(dout,800*600*3);
			escreveIntInvertido(dout,0);
			escreveIntInvertido(dout,0);
			escreveIntInvertido(dout,0);
			escreveIntInvertido(dout,0);

			for(int i = 0; i < 600; i++)
			{
				int cor = ((600-i)*0xff)/600;
				for( int j = 0; j < 800; j++)
				{
					dout.write(cor);
					dout.write(cor);
					dout.write(0);
				}
			}

			dout.close();
			saida.close();
		}
		catch(IOException ex)
		{
			ex.printStackTrace();
		}

	}

}
