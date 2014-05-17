#include "StdAfx.h"
#include "DuiObject.h"

// DUI����ID�Զ����ɱ�����DUI����ID��1000��ʼ���ɣ�DUI�ؼ�ID�Զ����ɵ���ʼֵ������Ķ���ID�ǲ�һ���ģ�
static int g_nDuiObjlId = 1000;

CDuiObject::CDuiObject(void)
{
	m_uID = g_nDuiObjlId++;
	m_strName = _T("");
	m_pDuiHandler = NULL;
	SetRect(CRect(0,0,0,0));
}

CDuiObject::~CDuiObject(void)
{
}

// �ж��Ƿ�˶���
BOOL CDuiObject::IsThisObject(UINT uID, CString strName)
{
	if(uID == 0 || strName == _T(""))
	{
		return FALSE;
	}

	if(uID == m_uID || strName == m_strName)
	{
		return TRUE;
	}

	return FALSE;
}

// ע���¼���������
BOOL CDuiObject::RegisterHandler(CDuiHandler* pDuiHandler)
{
	if(pDuiHandler == NULL)
	{
		return FALSE;
	}

	m_pDuiHandler = pDuiHandler;
	m_pDuiHandler->SetDuiObject(this);

	return TRUE;
}

// ���ö������ԵĻ�������������ID��name���Ե�����
HRESULT CDuiObject::SetAttribute(CStringA strAttribName, CStringA strValue, BOOL bLoading)
{
	HRESULT hRet = E_FAIL;
	if ("id" == strAttribName)
	{
		m_uID = ::StrToIntA(strValue);
		hRet = S_FALSE;
	}else
	if ("name" == strAttribName)
	{
		m_strName = CA2T(strValue, CP_UTF8);
		hRet = S_FALSE;
	}else
		return E_FAIL;

    return hRet;
}

// ����XML�ڵ㣬�����ڵ��е�������Ϣ���õ���ǰ�ؼ���������
BOOL CDuiObject::Load(TiXmlElement* pXmlElem, BOOL bLoadSubControl)
{
    for (TiXmlAttribute *pAttrib = pXmlElem->FirstAttribute(); NULL != pAttrib; pAttrib = pAttrib->Next())
    {
        SetAttribute(pAttrib->Name(), pAttrib->Value(), TRUE);
    }

    return TRUE;
}

// �����ַ������滻���е��滻����
void CDuiObject::ParseDuiString(CStringA& strString)
{
	DuiSystem::Instance()->ParseDuiString(strString);
}

ULONG CDuiObject::HexStringToULong(LPCSTR lpszValue, int nSize)
{
    LPCSTR pchValue = lpszValue;
    ULONG ulValue = 0;

    while (*pchValue && nSize != 0)
    {
        ulValue <<= 4;

        if ('a' <= *pchValue && 'f' >= *pchValue)
            ulValue |= (*pchValue - 'a' + 10);
        else if ('A' <= *pchValue && 'F' >= *pchValue)
            ulValue |= (*pchValue - 'A' + 10);
        else if ('0' <= *pchValue && '9' >= *pchValue)
            ulValue |= (*pchValue - '0');
        else
            return 0;

        ++ pchValue;
        -- nSize;
    }

    return ulValue;
}

// 16�����ַ���ת��ΪColor����
Color CDuiObject::HexStringToColor(LPCSTR lpszValue)
{
    return Color(
        HexStringToULong(lpszValue, 2), 
        HexStringToULong(lpszValue + 2, 2), 
        HexStringToULong(lpszValue + 4, 2)
        );
}

// 10���ƶ��ŷָ��ַ���ת��ΪColor����
Color CDuiObject::StringToColor(LPCSTR lpszValue)
{
	CStringA strValue = lpszValue;
	BYTE c1,c2,c3,c4;
	CStringA s1 = "";
	CStringA s2 = "";
	CStringA s3 = "";
	CStringA s4 = "";
	int nPos = strValue.Find(",");
	if(nPos != -1)
	{
		s1 = strValue.Left(nPos);
		strValue.Delete(0, nPos+1);
		nPos = strValue.Find(",");
		if(nPos != -1)
		{
			s2 = strValue.Left(nPos);
			strValue.Delete(0, nPos+1);
			nPos = strValue.Find(",");
			if(nPos != -1)
			{
				s3 = strValue.Left(nPos);
				strValue.Delete(0, nPos+1);
				s4 = strValue;
			}else
			{
				s3 = strValue;
			}
		}
	}

	c1 = atoi(s1);
	c2 = atoi(s2);
	c3 = atoi(s3);
	c4 = atoi(s4);

	if(s4.IsEmpty())
	{
		return Color(c1, c2, c3);
	}else
	{
		return Color(c1, c2, c3, c4);
	}
}

// 16�����ַ���תRGB��ɫ
COLORREF CDuiObject::HexStringToRGBColor(LPCSTR lpszValue)
{
    return RGB(
        HexStringToULong(lpszValue, 2), 
        HexStringToULong(lpszValue + 2, 2), 
        HexStringToULong(lpszValue + 4, 2)
        );
}

// �����ַ�����ȡ������
void CDuiObject::ParseKeyCode(LPCSTR lpszValue, UINT& nChar, UINT& nFlag)
{
	CStringA strValue = lpszValue;
	nChar = 0;
	nFlag = 0;
	strValue.Trim();
	strValue.MakeUpper();
	CStringA strFlag = "";
	CStringA strChar = strValue;
	int nPos = strValue.Find("+");
	if(nPos != -1)
	{
		strFlag = strValue.Left(nPos);
		strValue.Delete(0, nPos+1);
		strChar = strValue;
		strFlag.Trim();
		strChar.Trim();
	}

	if(strChar.IsEmpty())
	{
		return;
	}

	if(strFlag == "CTRL")
	{
		nFlag |= VK_CONTROL;
	}else
	if(strFlag == "ALT")
	{
		nFlag |= VK_MENU;
	}else
	if(strFlag == "SHIFT")
	{
		nFlag |= VK_SHIFT;
	}

	if(strChar == "RETURN")
	{
		nChar = VK_RETURN;
	}else
	if(strChar == "ESC")
	{
		nChar = VK_ESCAPE;
	}else
	if(strChar == "BACK")
	{
		nChar = VK_BACK;
	}else
	if(strChar == "TAB")
	{
		nChar = VK_TAB;
	}else
	if(strChar == "SPACE")
	{
		nChar = VK_SPACE;
	}else
	if(strChar == "PRIOR")
	{
		nChar = VK_PRIOR;
	}else
	if(strChar == "NEXT")
	{
		nChar = VK_NEXT;
	}else
	if(strChar == "END")
	{
		nChar = VK_END;
	}else
	if(strChar == "HOME")
	{
		nChar = VK_HOME;
	}else
	if(strChar == "LEFT")
	{
		nChar = VK_LEFT;
	}else
	if(strChar == "UP")
	{
		nChar = VK_UP;
	}else
	if(strChar == "RIGHT")
	{
		nChar = VK_RIGHT;
	}else
	if(strChar == "DOWN")
	{
		nChar = VK_DOWN;
	}else
	if(strChar == "SELECT")
	{
		nChar = VK_SELECT;
	}else
	if(strChar == "PRINT")
	{
		nChar = VK_PRINT;
	}else
	if(strChar == "INSERT")
	{
		nChar = VK_INSERT;
	}else
	if(strChar == "DELETE")
	{
		nChar = VK_DELETE;
	}else
	if(strChar == "F1")
	{
		nChar = VK_F1;
	}else
	if(strChar == "F2")
	{
		nChar = VK_F2;
	}else
	if(strChar == "F3")
	{
		nChar = VK_F3;
	}else
	if(strChar == "F4")
	{
		nChar = VK_F4;
	}else
	if(strChar == "F5")
	{
		nChar = VK_F5;
	}else
	if(strChar == "F6")
	{
		nChar = VK_F6;
	}else
	if(strChar == "F7")
	{
		nChar = VK_F7;
	}else
	if(strChar == "F8")
	{
		nChar = VK_F8;
	}else
	if(strChar == "F9")
	{
		nChar = VK_F9;
	}else
	if(strChar == "F10")
	{
		nChar = VK_F10;
	}else
	if(strChar == "F11")
	{
		nChar = VK_F11;
	}else
	if(strChar == "F12")
	{
		nChar = VK_F12;
	}else
	{
		char ch = strChar[0];
		if(((ch >= '0') && (ch < '9')) || ((ch >= 'A') && (ch < 'Z')))
		{
			nChar = ch;
		}
	}
}