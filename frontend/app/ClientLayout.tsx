'use client';

import Cookies from "js-cookie";
import {useEffect, useState} from "react";
import {BACKEND_ADDR} from "@/next.config";
import { ToastMessage } from "@/app/ToastMessage";

type status_code = "ok" | "error" | "warning" | "info";

export default function ClientLayout({ children }: { children: React.ReactNode }) {
	const [toast, setToast] = useState<{ status: status_code; message: string } | null>(null);

	const showToast = (status: status_code, message: string) => {
		setToast({ status, message });
	};

	useEffect(() => {
		const isLoggedIn = async () => {
			const token = Cookies.get("token");
			const email = Cookies.get("email");

			if (!token || !email) {
				console.log("Not logged in.");
				if (window.location.pathname != "/login"){
					window.location.href = "/login";
				}
				return false;
			}

			try {
				const res = await fetch(`${BACKEND_ADDR}/api/v1/verify_token`, {
					method: 'POST',
					headers: {
						'Content-Type': 'application/json',
						'X-Client-Email': String(email),
						'Authorization': String(token),
					},
				});

				if (!res.ok) {
					const err = await res.json();
					showToast("error", "Could not fetch credentials from server")
				} else {
					const resp = await res.json();
					showToast("ok", "Successfully logged in.")

					if (!resp){
						if (window.location.pathname != "/login"){
							window.location.href = "/login";
						}
					}
					return true;
				}

			} catch (err: any) {
				showToast("error", "Could not log in. Please re-check your credentials and try again")
				return false;
			}
		};

		isLoggedIn();
	}, []);

	return (
		<>{children}
						{toast && (
				<ToastMessage
					status={toast.status}
					message={toast.message}
					onClose={() => setToast(null)}
				/>
			)}
	</>);
}
