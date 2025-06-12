'use client'

import { BACKEND_ADDR } from "@/next.config";
import Cookies from "js-cookie";
import { ToastMessage } from "@/app/ToastMessage";
import {useState} from "react";

export default function Home(){
	type status_code = "ok" | "error" | "warning" | "info";
	const [toast, setToast] = useState<{ status: status_code; message: string } | null>(null);

	const showToast = (status: status_code, message: string) => {
		setToast({ status, message });
	};


	async function recalculate(){
		try {
			const res = await fetch(`${BACKEND_ADDR}/api/v1/recalculate`, {
				method: 'POST',
				headers: {
					'Content-Type': 'application/json',
					'X-Client-Email': String(Cookies.get('email')), // string to convert from undefined to "undefined"?
					'Authorization': String(Cookies.get('token'))
				},
			});

			if (!res.ok) {
				const err = await res.json();
				showToast("error", err);
			} else{
				showToast("ok", "Successfully recalculated output file.")
			}

		} catch (err: any) {
			showToast("error", err)
		}

	}


	async function downloadOutput() {
		try {
			const res = await fetch(`${BACKEND_ADDR}/api/v1/get-output-file`, {
				method: 'POST',
				headers: {
					'X-Client-Email': String(Cookies.get('email')),
					'Authorization': String(Cookies.get('token'))
				},
			});

			if (!res.ok) {
				const errText = await res.text();  // Might not be JSON
				showToast("error", "Download failed: " + errText);
			} else {
				showToast("ok", "Download occurred successfully.");
			}

			const blob = await res.blob();
			const url = window.URL.createObjectURL(blob);
			const a = document.createElement('a');

			// You can specify the filename here:
			a.href = url;
			a.download = 'output.csv';
			document.body.appendChild(a);
			a.click();

			// Cleanup
			a.remove();
			window.URL.revokeObjectURL(url);

		} catch (err: any) {
			showToast("error", "Download error: " + err)
		}
	}


	return (
		<div>
			<div className="flex justify-center items-center w-screen h-screen">
				<div className="relative text-center">
					<button className="bg-yellow-600 text-white px-4 py-1 rounded hover:bg-yellow-700 block my-6 mx-auto"
							onClick={downloadOutput}>Download Output File
					</button>

					<button className="bg-yellow-600 text-white px-4 py-1 rounded hover:bg-yellow-700 block my-6 mx-auto"
							onClick={recalculate}>Recalculate
					</button>

					<button className="bg-yellow-600 text-white px-4 py-1 rounded hover:bg-yellow-700 block my-6 mx-auto"
							onClick={e => window.location.href = "/edit"}>Edit
					</button>

				</div>
			</div>

			<ToastMessage status="ok" message="Successfull log in" onClose={() => setToast(null)}/>

		{toast && (
				<ToastMessage
					status={toast.status}
					message={toast.message}
					onClose={() => setToast(null)}
				/>
			)}
		</div>
	)
}