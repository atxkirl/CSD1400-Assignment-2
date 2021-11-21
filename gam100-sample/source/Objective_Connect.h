#pragma once
void OB_ConnectInit();
void OB_ConnectUpdate();
void OB_ConnectToggleActive();
int OB_ConnectIsComplete();
int OB_ConnectGetActive();
void OB_ConnectSetActive(int iSetter);

void OB_ConnectTrigger();
void OB_ConnectUnTrigger();

void CreateConnectors();