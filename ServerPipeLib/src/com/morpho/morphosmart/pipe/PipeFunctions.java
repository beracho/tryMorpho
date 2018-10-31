package com.morpho.morphosmart.pipe;

import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.InetAddress;
import java.net.Socket;
import java.net.SocketException;
import java.net.SocketTimeoutException;
import java.nio.ByteBuffer;

/**
 * Created by GT909287 on 14/11/2017.
 */

public class PipeFunctions implements IMsoPipeConstants {

    private Socket socket;
    private boolean finish = false;
    
    public void setFinish(boolean finish) {
		this.finish = finish;
	}


    public Socket getSocket() {
        return socket;
    }

    public void setSocket(Socket socket) {
        this.socket = socket;
    }

    /**
     * Sp_Pipe_Receive_LenAndDatas read length and Data
     * @param holder Holder of data
     * @return 0 on success, 1 otherwise
     */
    public int Sp_Pipe_Receive_LenAndDatas(Holder<byte[]> holder) {

        byte[] len = new byte[4];
        int receivedLen = 0;

        // Len Reception (4 bytes)
        int l_i_Ret = TCP_Recv(len, 4, MAX_RECV_TIME);
        if(l_i_Ret == 0) {
            // convert bytes to int
            receivedLen = reverse4Byte(len);
            
            if(receivedLen == 0) {
                holder.data = null;
                return l_i_Ret;
            }
            
            holder.data = new byte[receivedLen];
            
            l_i_Ret = TCP_Recv(holder.data, receivedLen, MAX_RECV_TIME );
        }

        return l_i_Ret;
    }

    public int Sp_Pipe_Receive_Status() {
        int		l_i_Ret;
        byte[] data = new byte[4];

        // Receive the Status (4 bytes) of the distant Write to the Mso
        l_i_Ret = TCP_Recv ( data, 4, MAX_RECV_TIME);
        if ( l_i_Ret == 0 ) {
            l_i_Ret = reverse4Byte(data);
        }

        return l_i_Ret;
    }

    public int Sp_Pipe_Receive_Responses(Holder<byte[]> holder, int desiredTag, int i_ul_Timeout) {
        int				l_i_Ret;
        byte			l_c_Tag;

        byte[] data = new byte[10];

        // We need a loop to discard the not desired response.
        while ( true )
        {
            l_i_Ret = TCP_Recv ( data, 1, i_ul_Timeout );
            l_c_Tag = data[0];

            if ( l_i_Ret != 0 )
            {
            	System.out.println( "PIEP_FUNCTIONS : Sp_Pipe_Receive_Responses TCP_Recv Err: " + l_i_Ret );
                break;
            }
            
            System.out.println( "PIEP_FUNCTIONS : Sp_Pipe_Receive_Responses received type " + l_c_Tag );
            
            if ( l_c_Tag == SP_PIPE_TAG_COMRECV)
            {
            	Sp_Pipe_Receive_LenAndDatas ( holder );
            	if ( l_c_Tag != desiredTag )
            		System.out.println( "PIEP_FUNCTIONS : l_c_Tag != desiredTag Sp_Pipe_Receive_Responses Sp_Pipe_Receive_LenAndDatas  " + holder.data.length );
            }

            if ( l_c_Tag == SP_PIPE_TAG_COMSEND )
            {
                // can discard the ComSend response, this arrives in case of asynchrone ILV reception during waiting a ILV_Cancel response.
                l_i_Ret = Sp_Pipe_Receive_Status (  );
                System.out.println( "PIEP_FUNCTIONS : Sp_Pipe_Receive_Responses Sp_Pipe_Receive_Status  " + l_i_Ret );
            }

            if ( l_c_Tag == SP_PIPE_TAG_CONNECT_MSO )
            {
                // can discard the ComSend response, this arrives in case of asynchrone ILV reception during waiting a ILV_Cancel response.
                l_i_Ret = Sp_Pipe_Receive_Status (  );
            }


            if ( desiredTag == l_c_Tag )
                break;
        }
        return l_i_Ret;
    }

    public int Sp_Pipe_Send_TagAndStatus(byte tag, int status) {

        byte[]	dataToSend = new byte[5];
        int		l_i_Ret;

        // Write Commande type 'TAG' transmission
        dataToSend[0] = tag;

        byte[] _status = reverseInt(status);
        for(int i = 0 ; i < _status.length ; i++) {
            dataToSend[i+1] = _status[i];
        }

        l_i_Ret = TCP_Send (dataToSend);

        return l_i_Ret;

    }

    public int Sp_Pipe_Send_LenAndDatas(byte[] data) {
        int	l_i_Ret = 0;
        byte[]	dataToSend = new byte[4];

        dataToSend = reverseInt(data.length);

        // Len transmission (4 bytes)
        if ( l_i_Ret == 0 )
        {
            l_i_Ret = TCP_Send ( dataToSend );
            if ( l_i_Ret != 0 )
            {
                //Sp_Pipe_LogText ( "Sends_LenAndDatas TCP_Send Err: %d", l_i_Ret );
            }
        }

        if ( l_i_Ret == 0 )
        {
            // Data transmission (Len bytes)
            l_i_Ret = TCP_Send ( data );
            if ( l_i_Ret != 0 )
            {
                //Sp_Pipe_LogText ( "Sends_LenAndDatas TCP_Send Err: %d", l_i_Ret );
            }
        }
        return l_i_Ret;
    }

    public int TCP_Recv(byte[] data, int lenToRecive, int i_ul_TimeOut) {
    	
    	if(finish) {
    		return -1;
    	}

        try {

        	long endTime = 0;        	
            
        	socket.setSoTimeout(i_ul_TimeOut);
            
            if(i_ul_TimeOut > 0) 
            	endTime = System.currentTimeMillis() + i_ul_TimeOut;

            InputStream dis = socket.getInputStream();
            
            int readed = 0;
            int temp;
            do {

                temp = dis.read(data, readed, lenToRecive-readed);
                
                if(temp > 0) {
                    readed += temp;
                }
                
                if(temp < 0) {
                	return -1;
                }
                
                if(i_ul_TimeOut > 0) {
                	if( endTime < System.currentTimeMillis() ) 
                		return -10000;
                }
                
            }while(readed < lenToRecive);

            if(readed >= lenToRecive) {
                return 0;
            }
        }catch(SocketTimeoutException e) {
            return -10000;
        } catch(SocketException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        } catch(Exception e) {
        	e.printStackTrace();
        }

        return 1;
    }

    public int TCP_Send(byte[] data) {
        try {
            DataOutputStream dos = new DataOutputStream(socket.getOutputStream());
            dos.write(data);
            
            if(data.length != dos.size()) {
            	System.out.println( "PIPE_FUNCTION ERROR: TCP_Send data.length " + data.length + " send: len " + dos.size());
            }
            //dos.flush();
            return 0;
        } catch (Exception e) {
            e.printStackTrace();
        }
        return 1;
    }
    
    public int TCP_Client_Close() {
        try {
        	if(socket != null)
        		socket.close();
            return 0;
        } catch (Exception e) {
        	System.out.println( "PIPE_FUNCTION ERROR: TCP_Client_Close " + e.getMessage());
        }
        return 1;
    }

    public int TCP_Client_Connect(String ip,int port) {

        try {
            socket = new Socket(InetAddress.getByName(ip), port);
            return 0;
        } catch (Exception e) {
        	System.out.println( "PIPE_FUNCTION ERROR: TCP_Client_Connect " + e.getMessage());
        }
        return 1;
    }

    public int reverse4Byte(byte[] data) {
        int lastIndex = 3;
        for(int i = 0 ; i < 2 ; i++)
        {
            byte tmp = data[i];
            data[i] = data[lastIndex-i];
            data[lastIndex-i] = tmp;
        }

        return ByteBuffer.wrap(data).getInt();
    }

    public byte[] reverseInt(int value) {
        byte[] l_i_Ret_byte = ByteBuffer.allocate(4).putInt(value).array();
        byte[] res = new byte[4];
        for (int i = 0; i < 4; i++) {
            res[i] = l_i_Ret_byte[3 - i];
        }
        return res;
    }
}
