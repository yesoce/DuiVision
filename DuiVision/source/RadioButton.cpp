#include "StdAfx.h"
#include "RadioButton.h"

CDuiRadioButton::CDuiRadioButton(HWND hWnd, CDuiObject* pDuiObject)
						   : CControlBaseFont(hWnd, pDuiObject)
{
	m_bTabStop = TRUE;	// ������Ӧtab��
	m_bIsFocus = FALSE;
	m_enButtonState = enBSNormal;
	m_pImage = NULL;
	m_bDown = false;
	m_bMouseDown = false;
	m_uVAlignment = DT_VCENTER;
	m_clrText = Color(254, 0, 0, 0);
	m_strGroupName = _T("");
	m_strValue = _T("");
	SetBitmapCount(6);
}

CDuiRadioButton::CDuiRadioButton(HWND hWnd, CDuiObject* pDuiObject, UINT uControlID, CRect rc, CString strTitle/*= TEXT("")*/, BOOL bIsVisible/* = TRUE*/, 
						   BOOL bIsDisable/* = FALSE*/ ,BOOL bIsPressDown/* = FALSE*/)
						   : CControlBaseFont(hWnd, pDuiObject, uControlID, rc, strTitle, bIsVisible, bIsDisable)
{
	m_bTabStop = TRUE;	// ������Ӧtab��
	m_bIsFocus = FALSE;
	m_enButtonState = enBSNormal;
	m_pImage = NULL;
	m_bDown = false;
	m_bMouseDown = false;
	m_uVAlignment = DT_VCENTER;
	m_clrText = Color(254, 0, 0, 0);
	m_strGroupName = _T("");
	m_strValue = _T("");
	SetBitmapCount(6);
}

CDuiRadioButton::~CDuiRadioButton(void)
{
}

// ��ȡ�Ƿ�ѡ���״̬
BOOL CDuiRadioButton::GetCheck()
{
	return m_bDown;
}

// ����ѡ��״̬
BOOL CDuiRadioButton::SetCheck(BOOL bCheck)
{
	if (m_bIsDisable)
	{
		return m_bDown;
	}

	BOOL bDown = m_bDown;
	m_bDown = bCheck;
	if(m_bDown)
	{
		m_enButtonState = enBSDown;
	}
	else
	{
		 m_enButtonState = enBSNormal;
	}
	if(m_bDown != bDown)
	{
		// ˢ��ͬһ������������radiobtn
		ResetGroupCheck();
		UpdateControl();
	}
	return m_bDown;
}

// ���ÿؼ���ѡ��״̬,����ȥ���������ؼ���״̬
void CDuiRadioButton::SetControlCheck(BOOL bCheck)
{
	m_bDown = bCheck;
	m_enButtonState = bCheck ? enBSDown : enBSNormal;
	UpdateControl();
}

// ˢ�¸��ؼ���������ͬһ�����RadioButton�ؼ���״̬
BOOL CDuiRadioButton::ResetGroupCheck()
{
	CDuiObject* pParentObj = GetParent();
	if(pParentObj == NULL)
	{
		return FALSE;
	}

	vector<CControlBase*>* pvecControl = NULL;
	if(pParentObj->IsClass("dlg"))
	{
		CDlgBase* pDlg = static_cast<CDlgBase*>(pParentObj);
		pvecControl = pDlg->GetControls();
	}else
	if(pParentObj->IsClass("popup"))
	{
		CDlgPopup* pDlg = static_cast<CDlgPopup*>(pParentObj);
		pvecControl = pDlg->GetControls();
	}else
	{
		CControlBase* pControlBase = static_cast<CControlBase*>(pParentObj);
		pvecControl = pControlBase->GetControls();
	}

	if(pvecControl == NULL)
	{
		return FALSE;
	}

	for(int i=0; i<pvecControl->size(); i++)
	{
		CControlBase* pControlBase = pvecControl->at(i);
		if(pControlBase->IsClass(CDuiRadioButton::GetClassName()) && pControlBase->GetVisible() && !pControlBase->GetDisable())
		{
			CDuiRadioButton* pControl = static_cast<CDuiRadioButton*>(pControlBase);
			if((pControl->GetGroupName() == m_strGroupName) && (pControl != this))
			{
				// ���ÿؼ�״̬
				pControl->SetControlCheck(FALSE);
			}
		}
	}

	return TRUE;
}

// ��ȡ����Radio��ѡ��Ŀؼ���ֵ
CString CDuiRadioButton::GetGroupValue()
{
	CDuiObject* pParentObj = GetParent();
	if(pParentObj == NULL)
	{
		return _T("");
	}

	vector<CControlBase*>* pvecControl = NULL;
	if(pParentObj->IsClass("dlg"))
	{
		CDlgBase* pDlg = static_cast<CDlgBase*>(pParentObj);
		pvecControl = pDlg->GetControls();
	}else
	if(pParentObj->IsClass("popup"))
	{
		CDlgPopup* pDlg = static_cast<CDlgPopup*>(pParentObj);
		pvecControl = pDlg->GetControls();
	}else
	{
		CControlBase* pControlBase = static_cast<CControlBase*>(pParentObj);
		pvecControl = pControlBase->GetControls();
	}

	if(pvecControl == NULL)
	{
		return _T("");
	}

	for(int i=0; i<pvecControl->size(); i++)
	{
		CControlBase* pControlBase = pvecControl->at(i);
		if(pControlBase->IsClass(CDuiRadioButton::GetClassName()) && pControlBase->GetVisible() && !pControlBase->GetDisable())
		{
			CDuiRadioButton* pControl = static_cast<CDuiRadioButton*>(pControlBase);
			if((pControl->GetGroupName() == m_strGroupName) && pControl->GetCheck())
			{
				return pControl->GetValue();
			}
		}
	}

	return _T("");
}

// ��XML����check����
HRESULT CDuiRadioButton::OnAttributeCheck(const CStringA& strValue, BOOL bLoading)
{
	if (strValue.IsEmpty()) return E_FAIL;

	BOOL bCheck = (strValue == "true");
	SetCheck(bCheck);

	return bLoading?S_FALSE:S_OK;
}

// ���ÿؼ��Ľ���
BOOL CDuiRadioButton::SetControlFocus(BOOL bFocus)
{
	__super::SetControlFocus(bFocus);

	if(!GetVisible() || GetDisable())
	{
		return FALSE;
	}

	m_bIsFocus = bFocus;
	UpdateControl(true);

	return TRUE;
}

BOOL CDuiRadioButton::OnControlMouseMove(UINT nFlags, CPoint point)
{
	enumButtonState buttonState = m_enButtonState;
	if (!m_bIsDisable && !m_bMouseDown)
	{
		if(m_rc.PtInRect(point))
		{
			if(m_bDown)
			{
				m_enButtonState = enBSHoverDown;
			}
			else
			{
				m_enButtonState = enBSHover;
			}
		}
		else
		{
			if(m_bDown)
			{
				m_enButtonState = enBSDown;
			}
			else
			{
				m_enButtonState = enBSNormal;
			}
		}
	}
	
	if(buttonState != m_enButtonState)
	{
		UpdateControl();
		return true;
	}
	return false;
}

BOOL CDuiRadioButton::OnControlLButtonDown(UINT nFlags, CPoint point)
{
	enumButtonState buttonState = m_enButtonState;
	if (!m_bIsDisable)
	{
		if(m_rc.PtInRect(point))
		{
			m_bMouseDown = true;
			if(m_bDown)
			{
				m_enButtonState = enBSHoverDown;
				// ����Ϊ��ǰ���ڼ���״̬
				m_bIsFocus = TRUE;
			}
			else
			{
				m_enButtonState = enBSHover;
			}
		}
	}
	
	if(buttonState != m_enButtonState)
	{
		UpdateControl();
		return true;
	}
	return false;
}

BOOL CDuiRadioButton::OnControlLButtonUp(UINT nFlags, CPoint point)
{
	enumButtonState buttonState = m_enButtonState;
	if (!m_bIsDisable)
	{
		if(m_rc.PtInRect(point))
		{
			if(m_bMouseDown)
			{
				m_bDown = true;
				// ˢ��ͬһ������������radiobtn
				ResetGroupCheck();
				SendMessage(BUTTOM_UP, 0, 0);
			}
			if(m_bDown)
			{
				m_enButtonState = enBSHoverDown;
			}
			else
			{
				m_enButtonState = enBSHover;
			}			
		}
		else
		{
			if(m_bDown)
			{
				m_enButtonState = enBSDown;
			}
			else
			{
				m_enButtonState = enBSNormal;
			}			
		}
	}
	m_bMouseDown = false;
	
	if(buttonState != m_enButtonState)
	{
		UpdateControl();
		return true;
	}
	return false;
}

// �����¼�����
BOOL CDuiRadioButton::OnControlKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// �����ǰ���ڽ���״̬,�ÿո�������л�check
	if(m_bIsFocus && (nChar == VK_SPACE) && (nFlags == 0))
	{
		SetCheck(TRUE);
		return true;
	}

	return false;
}

void CDuiRadioButton::SetControlDisable(BOOL bIsDisable)
{
	if(m_bIsDisable != bIsDisable)
	{
		m_bIsDisable = bIsDisable;
		if(bIsDisable)
		{
			if(m_bDown)
			{
				m_enButtonState = enBSDisableDown;
			}
			else
			{
				m_enButtonState = enBSDisable;
			}
		}
		else
		{
			if(m_bDown)
			{
				m_enButtonState = enBSDown;
			}
			else
			{
				m_enButtonState = enBSNormal;
			}
		}
		::InvalidateRect(m_hWnd, &m_rc, true);
	}
}

void CDuiRadioButton::DrawControl(CDC &dc, CRect rcUpdate)
{
	int nWidth = m_rc.Width();
	int nHeight = m_rc.Height();

	if(!m_bUpdate)
	{
		UpdateMemDC(dc, nWidth * 6, nHeight);

		Graphics graphics(m_memDC);
		CRect  rcTemp(0, 0, nWidth, nHeight);

		for(int i = 0; i < 6; i++)
		{
			m_memDC.BitBlt(i * nWidth, 0, nWidth, nHeight, &dc, m_rc.left ,m_rc.top, SRCCOPY);

			graphics.DrawImage(m_pImage, Rect(rcTemp.left, rcTemp.top + (nHeight - m_sizeImage.cy) / 2,   m_sizeImage.cx, m_sizeImage.cy),
				i * m_sizeImage.cx, 0, m_sizeImage.cx, m_sizeImage.cy, UnitPixel);

			rcTemp.OffsetRect(nWidth, 0);
		}
		
		if(!m_strTitle.IsEmpty())
		{
			m_memDC.SetBkMode(TRANSPARENT);

			rcTemp.SetRect(0, 0, nWidth, nHeight);

			FontFamily fontFamily(m_strFont.AllocSysString());
			Font font(&fontFamily, (REAL)m_nFontWidth, m_fontStyle, UnitPixel);
			graphics.SetTextRenderingHint( TextRenderingHintClearTypeGridFit );

			StringFormat strFormat;
			strFormat.SetAlignment(StringAlignmentNear);
			strFormat.SetFormatFlags( StringFormatFlagsNoWrap | StringFormatFlagsMeasureTrailingSpaces);
			Size size = GetTextBounds(font, strFormat, m_strTitle);
			CPoint point = GetOriginPoint(nWidth - m_sizeImage.cx - 3, nHeight, size.Width, size.Height, m_uAlignment, m_uVAlignment);

			for(int i = 0; i < 6; i++)
			{
				SolidBrush solidBrush(enBSDisable == i ? Color(128, 128, 128) : m_clrText);

				RectF rect(m_sizeImage.cx + 3 + point.x + i * nWidth, point.y, nWidth - m_sizeImage.cx - 3 - point.x, size.Height);
				graphics.DrawString(m_strTitle.AllocSysString(), (INT)wcslen(m_strTitle.AllocSysString()), &font, 
					rect, &strFormat, &solidBrush);

				// �������(���߿�)
				if(m_bIsFocus)
				{
					Pen pen(Color(128, 128, 128), 1);
					pen.SetDashStyle(DashStyleDot);
					RectF rectFocus(point.x + i * nWidth, point.y, m_sizeImage.cx + 6 + size.Width, size.Height);
					graphics.DrawRectangle(&pen, rectFocus);
				}
			}
		}
	}

	dc.BitBlt(m_rc.left,m_rc.top, m_rc.Width(), m_rc.Height(), &m_memDC, m_enButtonState * nWidth, 0, SRCCOPY);
}