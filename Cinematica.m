clf
clc
clear all

L1=1;
L2=20;
L3=20;
L4=20;
L5=20;
L6=10;
Tool=2;

% L(1)=Link([0 0 L1 -pi/2]);
% L(2)=Link([0 0 L2 pi/2]);
% L(3)=Link([0 0 L3 pi/2]);
% L(4)=Link([-pi/2 0 L4 pi/2]);
% L(5)=Link([0 0 L5 pi/2]);
% L(6)=Link([0 pi/2 0 0]);

% % L(1)=Link([0 0 L1 -pi/2]);
% % L(2)=Link([0 0 L2 0]);
% % L(3)=Link([0 0 L3 pi/2]);
% % L(4)=Link([0 0 L4 pi/2]);
% % L(5)=Link([pi/2 0 L5 pi/2]);
% % L(6)=Link([0 L6 0 0]);

% L(1)=Link([0 0 L1 -pi/2]);
% L(2)=Link([0 0 L2 0]);
% L(3)=Link([0 0 L3 pi/2]);
% L(4)=Link([-pi/2 0 L4 pi/2]);
% L(5)=Link([pi/2 0 L5 0]);
% L(6)=Link([0 L6 0 0]);

L(1)=Link('revolute','alpha',pi/2,'a',0,'d',0,'offset',0,'modified');
L(2)=Link('revolute','alpha',0,'a',10,'d',0,'offset',0,'modified');
L(3)=Link('revolute','alpha',0,'a',L3,'d',0,'offset',0,'modified');
L(4)=Link('revolute','alpha',0,'a',0,'d',L4,'offset',0,'modified');
L(5)=Link('revolute','alpha',0,'a',L5,'d',0,'offset',0,'modified');
L(6)=Link('revolute','alpha',0,'a',L6,'d',0,'offset',0,'modified');

Robot_6R=SerialLink(L,'name','Robot_6R');
Robot_6R.tool = transl(0,0,Tool);

q = [pi/2 0 pi/2 pi/2 pi 0];
% Robot_6R.plot(q,'workspace',[-30 30 -30 30 0 30],'tilesize',20,'scale',1);
Robot_6R.plot(q,'workspace',[-100 100 -100 100 -30 100]);
hold on
trplot(eye(4),'rgb','length',8,'frame','0')
Robot_6R.teach()
axis([-100 100 -100 100 -30 100])
view(50,20)
%%
D2 = [-1 0 0 49.28;0 1 0 0;0 0 -1 50;0 0 0 1]
q = [0 -pi/2 pi/4 0 -pi/4 pi];
D1=Robot_6R.fkine(q)
Q=Robot_6R.ikunc(D2)
rad2deg(Q)
%%
clf
Robot_6R.plot(Q,'workspace',[-100 100 -100 100 -30 100]);
hold on
trplot(eye(4),'rgb','length',8,'frame','0')
Robot_6R.teach()
axis([-100 100 -100 100 -30 100])
view(50,20)