#include "stdafx.h"
#include "CommInfo.h"

CommInfo::CommInfo()
{
	
}

CommInfo::CommInfo(int nCom, int nBaud, char cParity, int nDatabits, int nStopbits)
{
	this->m_nCom = nCom;
	this->m_nBaud = nBaud;
	this->m_cParity = cParity;
	this->m_nDatabits = nDatabits;
	this->m_nStopbits = nStopbits;
}

CommInfo::~CommInfo()
{	
}

void CommInfo::setBand(int nBaud)
{
	this->m_nBaud = nBaud;
}
	
int CommInfo::getBand()
{
	return this->m_nBaud;
}

void CommInfo::setCom(int nCom)
{
	this->m_nCom = nCom;
}
	
int CommInfo::getCom()
{
	return this->m_nCom;
}

void CommInfo::setParity(char cParity)
{
	this->m_cParity = cParity;
}

char CommInfo::getParity()
{
	return this->m_cParity;
}

void CommInfo::setDatabits(int nDatabits)
{
	this->m_nDatabits = nDatabits;
}

int CommInfo::getDatabits()
{
	return this->m_nDatabits;
}

void CommInfo::setStopbits(int nStopbits)
{
	this->m_nStopbits = nStopbits;
}

int CommInfo::getStopbits()
{
	return this->m_nStopbits;
}



