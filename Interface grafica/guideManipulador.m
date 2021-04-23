function varargout = guideManipulador(varargin)
% GUIDEMANIPULADOR MATLAB code for guideManipulador.fig
%      GUIDEMANIPULADOR, by itself, creates a new GUIDEMANIPULADOR or raises the existing
%      singleton*.
%
%      H = GUIDEMANIPULADOR returns the handle to a new GUIDEMANIPULADOR or the handle to
%      the existing singleton*.
%
%      GUIDEMANIPULADOR('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in GUIDEMANIPULADOR.M with the given input arguments.
%
%      GUIDEMANIPULADOR('Property','Value',...) creates a new GUIDEMANIPULADOR or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before guideManipulador_OpeningFcn gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to guideManipulador_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to help guideManipulador

% Last Modified by GUIDE v2.5 28-Jul-2020 15:50:15

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @guideManipulador_OpeningFcn, ...
                   'gui_OutputFcn',  @guideManipulador_OutputFcn, ...
                   'gui_LayoutFcn',  [] , ...
                   'gui_Callback',   []);
if nargin && ischar(varargin{1})
    gui_State.gui_Callback = str2func(varargin{1});
end

if nargout
    [varargout{1:nargout}] = gui_mainfcn(gui_State, varargin{:});
else
    gui_mainfcn(gui_State, varargin{:});
end
% End initialization code - DO NOT EDIT


% --- Executes just before guideManipulador is made visible.
function guideManipulador_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to guideManipulador (see VARARGIN)

% Choose default command line output for guideManipulador
handles.output = hObject;

% Update handles structure
guidata(hObject, handles);

% UIWAIT makes guideManipulador wait for user response (see UIRESUME)
% uiwait(handles.figure1);


% --- Outputs from this function are returned to the command line.
function varargout = guideManipulador_OutputFcn(hObject, eventdata, handles) 
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;


% --- Executes on button press in hacer.
function hacer_Callback(hObject, eventdata, handles)
% hObject    handle to hacer (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
set(handles.as1,'String',get(handles.servo1,'value'));
set(handles.as2,'String',get(handles.servo2,'value'));
set(handles.as3,'String',get(handles.servo3,'value'));
set(handles.as4,'String',get(handles.servo4,'value'));
set(handles.as5,'String',get(handles.servo5,'value'));
set(handles.as6,'String',get(handles.servo6,'value'));


s1=get(handles.servo1,'value');
s2=get(handles.servo2,'value');
s3=get(handles.servo3,'value');
s4=get(handles.servo4,'value');
s5=get(handles.servo5,'value');
s6=get(handles.servo6,'value');
global th1 th2 th3 th4 th5 th6 q Robot 

th1=s1*pi/180;
th2=s2*pi/180;
th3=s3*pi/180;
th4=s4*pi/180;
th5=s5*pi/180;
th6=s6*pi/180;

L1=1;
L2=20;
L3=20;
L4=20;
L5=20;


L(1)=Link([0 0 L1 -pi/2]);
L(2)=Link([0 0 L2 0]);
L(3)=Link([0 0 L3 pi/2]);
L(4)=Link([-pi/2 0 L4 pi/2]);
L(5)=Link([pi/2 0 L5 0]);
L(6)=Link([0 0 0 -pi/2]);
Robot=SerialLink(L,'name','Robot');
q=[th1 th2 th3 th4 th5 th6];
D1=Robot.fkine(q)
Q=Robot.ikunc(D1)
Robot.plot(q,'workspace',[-100 100 -100 100 -30 100]);
envidata(s1,s2,s3,s4,s5,s6);


% --- If Enable == 'on', executes on mouse press in 5 pixel border.
% --- Otherwise, executes on mouse press in 5 pixel border or over hacer.
function hacer_ButtonDownFcn(hObject, eventdata, handles)
% hObject    handle to hacer (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)


% --- Executes on slider movement.
function servo1_Callback(hObject, eventdata, handles)
% hObject    handle to servo1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'Value') returns position of slider
%        get(hObject,'Min') and get(hObject,'Max') to determine range of slider
global th1 th2 th3 th4 th5 th6 q Robot
set(handles.as1,'String',get(hObject,'value'));
th1=get(hObject,'value')*pi/180;
q=[th1 th2 th3 th4 th5 th6];
Robot.plot(q,'workspace',[-100 100 -100 100 -30 100]);

% --- Executes during object creation, after setting all properties.
function servo1_CreateFcn(hObject, eventdata, handles)
% hObject    handle to servo1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: slider controls usually have a light gray background.
if isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor',[.9 .9 .9]);
end


% --- Executes on slider movement.
function servo2_Callback(hObject, eventdata, handles)
% hObject    handle to servo2 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'Value') returns position of slider
%        get(hObject,'Min') and get(hObject,'Max') to determine range of slider
global th1 th2 th3 th4 th5 th6 q Robot
set(handles.as2,'String',get(hObject,'value'));
th2=get(hObject,'value')*pi/180;
q=[th1 th2 th3 th4 th5 th6];
Robot.plot(q,'workspace',[-100 100 -100 100 -30 100]);

% --- Executes during object creation, after setting all properties.
function servo2_CreateFcn(hObject, eventdata, handles)
% hObject    handle to servo2 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: slider controls usually have a light gray background.
if isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor',[.9 .9 .9]);
end


% --- Executes on slider movement.
function servo3_Callback(hObject, eventdata, handles)
% hObject    handle to servo3 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'Value') returns position of slider
%        get(hObject,'Min') and get(hObject,'Max') to determine range of slider
global th1 th2 th3 th4 th5 th6 q Robot
set(handles.as3,'String',get(hObject,'value'));
th3=get(hObject,'value')*pi/180;
q=[th1 th2 th3 th4 th5 th6];
Robot.plot(q,'workspace',[-100 100 -100 100 -30 100]);

% --- Executes during object creation, after setting all properties.
function servo3_CreateFcn(hObject, eventdata, handles)
% hObject    handle to servo3 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: slider controls usually have a light gray background.
if isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor',[.9 .9 .9]);
end


% --- Executes on slider movement.
function servo4_Callback(hObject, eventdata, handles)
% hObject    handle to servo4 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'Value') returns position of slider
%        get(hObject,'Min') and get(hObject,'Max') to determine range of slider
global th1 th2 th3 th4 th5 th6 q Robot
set(handles.as4,'String',get(hObject,'value'));
th4=get(hObject,'value')*pi/180;
q=[th1 th2 th3 th4 th5 th6];
Robot.plot(q,'workspace',[-100 100 -100 100 -30 100]);

% --- Executes during object creation, after setting all properties.
function servo4_CreateFcn(hObject, eventdata, handles)
% hObject    handle to servo4 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: slider controls usually have a light gray background.
if isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor',[.9 .9 .9]);
end


% --- Executes on slider movement.
function servo5_Callback(hObject, eventdata, handles)
% hObject    handle to servo5 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'Value') returns position of slider
%        get(hObject,'Min') and get(hObject,'Max') to determine range of slider
global th1 th2 th3 th4 th5 th6 q Robot
set(handles.as5,'String',get(hObject,'value'));
th5=get(hObject,'value')*pi/180;
q=[th1 th2 th3 th4 th5 th6];
Robot.plot(q,'workspace',[-100 100 -100 100 -30 100]);



% --- Executes during object creation, after setting all properties.
function servo5_CreateFcn(hObject, eventdata, handles)
% hObject    handle to servo5 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: slider controls usually have a light gray background.
if isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor',[.9 .9 .9]);
end


% --- Executes on slider movement.
function servo6_Callback(hObject, eventdata, handles)
% hObject    handle to servo6 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'Value') returns position of slider
%        get(hObject,'Min') and get(hObject,'Max') to determine range of slider
set(handles.as6,'String',get(hObject,'value'));


% --- Executes during object creation, after setting all properties.
function servo6_CreateFcn(hObject, eventdata, handles)
% hObject    handle to servo6 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: slider controls usually have a light gray background.
if isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor',[.9 .9 .9]);
end



function edit1_Callback(hObject, eventdata, handles)
% hObject    handle to edit1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit1 as text
%        str2double(get(hObject,'String')) returns contents of edit1 as a double


% --- Executes during object creation, after setting all properties.
function edit1_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function as1_Callback(hObject, eventdata, handles)
% hObject    handle to as1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of as1 as text
%        str2double(get(hObject,'String')) returns contents of as1 as a double


% --- Executes during object creation, after setting all properties.
function as1_CreateFcn(hObject, eventdata, handles)
% hObject    handle to as1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function as2_Callback(hObject, eventdata, handles)
% hObject    handle to as2 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of as2 as text
%        str2double(get(hObject,'String')) returns contents of as2 as a double


% --- Executes during object creation, after setting all properties.
function as2_CreateFcn(hObject, eventdata, handles)
% hObject    handle to as2 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function as3_Callback(hObject, eventdata, handles)
% hObject    handle to as3 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of as3 as text
%        str2double(get(hObject,'String')) returns contents of as3 as a double


% --- Executes during object creation, after setting all properties.
function as3_CreateFcn(hObject, eventdata, handles)
% hObject    handle to as3 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function as4_Callback(hObject, eventdata, handles)
% hObject    handle to as4 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of as4 as text
%        str2double(get(hObject,'String')) returns contents of as4 as a double


% --- Executes during object creation, after setting all properties.
function as4_CreateFcn(hObject, eventdata, handles)
% hObject    handle to as4 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function as5_Callback(hObject, eventdata, handles)
% hObject    handle to as5 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of as5 as text
%        str2double(get(hObject,'String')) returns contents of as5 as a double


% --- Executes during object creation, after setting all properties.
function as5_CreateFcn(hObject, eventdata, handles)
% hObject    handle to as5 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function as6_Callback(hObject, eventdata, handles)
% hObject    handle to as6 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of as6 as text
%        str2double(get(hObject,'String')) returns contents of as6 as a double


% --- Executes during object creation, after setting all properties.
function as6_CreateFcn(hObject, eventdata, handles)
% hObject    handle to as6 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes during object creation, after setting all properties.
function figure1_CreateFcn(hObject, eventdata, handles)
% hObject    handle to figure1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called
clc
