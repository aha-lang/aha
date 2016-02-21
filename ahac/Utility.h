#pragma once

void replaceStr(std::wstring & strCallId, const wchar_t * pszBefore, const wchar_t * pszAfter)
{
	size_t iPos = strCallId.find(pszBefore);
	size_t iBeforeLen = wcslen(pszBefore);
	while (iPos < std::wstring::npos)
	{
		strCallId.replace(iPos, iBeforeLen, pszAfter);
		iPos = strCallId.find(pszBefore, iPos);
	}
}

bool superLetter(wchar_t ch)
{
	if (ch > 0xff) return true;
	else if (iswalpha(ch) || ch == L'_') return true;
	else return false;
}

bool superLetterOrDigit(wchar_t ch)
{
	if (ch > 0xff) return true;
	else if (iswalnum(ch) || ch == L'_') return true;
	else return false;
}