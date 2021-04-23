function a = envidata(s1,s2,s3,s4,s5,s6)
time=0.5;
  
oldobj = instrfind;
if~isempty(oldobj)
    fclose(oldobj);
    delete(oldobj);
end
%crea puerto serial
%s =serial('COM2','BaudRate',9600,'Databits',8,'Parity','None,'StopBits',1);
if ~exist('s','var')
   s=serial('COM8','BaudRate',9600,'Databits',8,'Parity','None','StopBits',1);
end
%apertura de comunicacion serial
if strcmp(get(s,'status'),'closed')
    fopen(s);
end
    DATA=[s1 s2 s3 s4 s5 s6];
    DATA=round(DATA);
    for i=1:6
            DATA(i)=DATA(i)+90;
            a0=floor(DATA(i)/100);
            a1=floor(mod(DATA(i),100)/10);
            dato=mod(DATA(i),100);
            a2=mod(dato,10);
            fwrite(s,'s','uchar')
            pause(time);
            fwrite(s,num2str(i-1),'uchar')
            pause(time);
            fwrite(s,'a','uchar')
            pause(time);
            fwrite(s,num2str(a0),'uchar')
            pause(time);
            fwrite(s,num2str(a1),'uchar')
            pause(time);
            fwrite(s,num2str(a2),'uchar')
            pause(time);
    end
    fclose(s);
