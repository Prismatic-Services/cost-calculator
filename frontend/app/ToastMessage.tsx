import { useEffect } from "react";

type status_code = "ok" | "error" | "warning" | "info";

type ToastMessageProps = {
	status: status_code;
	message: string;
	onClose: () => void;
};

export const ToastMessage = ({ status, message, onClose }: ToastMessageProps) => {
	const status_conversion = new Map<string, string>([
		["ok", "Success"],
		["warning", "Warning"],
		["info", "Information"],
		["error", "Error"]
	]);

	const status_colors = new Map<string, string>([
		["ok", "bg-green-500"],
		["warning", "bg-orange-500"],
		["info", "bg-yellow-500"],
		["error", "bg-red-500"]
	]);

	// Auto close after 10s
	useEffect(() => {
		const timer = setTimeout(() => onClose(), 5000);
		return () => clearTimeout(timer);
	}, [onClose]);

	return (
		<div className="h-28 w-72 absolute bottom-16 right-16 bg-white flex rounded-md justify-between shadow-lg z-50" onClick={onClose}>
			<div className={`${status_colors.get(status)} h-full w-4 rounded-tl-md rounded-bl-md`}></div>
			<div className="w-58">
				<h1 className="m-3 text-black text-2xl font-semibold">{status_conversion.get(status)}</h1>
				<p className="m-3 text-black text-sm">{message}</p>
			</div>
			<i className="fa-solid fa-x m-3 cursor-pointer" onClick={onClose}></i>
		</div>
	);
};
