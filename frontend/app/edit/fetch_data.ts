import Cookies from "js-cookie";
import {BACKEND_ADDR} from "@/next.config";

export async function fetch_file(filename: string): Promise<string | null> {
	try {
		const res = await fetch(`${BACKEND_ADDR}/api/v1/get_file?file=${filename}.csv`, {
			method: 'POST',
			headers: {
				'Content-Type': 'application/json',
				'X-Client-Email': String(Cookies.get('email')),
				'Authorization': String(Cookies.get('token'))
			},
		});

		if (!res.ok) {
			const err = await res.json();
			throw new Error(err.message || 'Fetch failed...');
		}

		const resp = await res.json();
		return String(resp);
	} catch (err: any) {
		console.error(err.message);
		return null;
	}
}


export function format_as_array(data: string): Array<Array<string>>{
	const lines: Array<string> = data.split('\n');
	let return_array: Array<Array<string>> = [];

	for (let i = 0; i < lines.length; i++) {
		return_array.push(lines[i].split(','));
	}

	return return_array;
}

export function assigning_keys(data: Array<Array<string>>): Map<string, string> {
	const map = new Map<string, string>();

	for (let row_idx=0; row_idx < data.length; row_idx++) {
		for (let col_idx=0; col_idx < data[row_idx].length; col_idx++) {
			map.set(`${row_idx}-${col_idx}`, String(data[row_idx][col_idx]));
		}
	}

	return map;
}